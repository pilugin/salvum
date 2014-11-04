#include <jpeg/picojpegdecodr.h>
#include <core-3/fetch.h>
#include <util/ilog.h>
#include <jpeg/icheck.h>

#include <functional>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutexLocker>
#include <QtDebug>

using namespace Log;
using namespace Core3;

namespace Jpeg {

DecodrState::DecodrState(QImage *image)
{
    pjpegCtxt.resize(pjpeg_ctxt_buffer_size);

    bufferPos = 0;
    bytesRead = 0;
    buffer.clear();
    lastWasFF = false;
    if (image) {
        cursor.setCanvas(image);
        cursor.restart();
    }
    decodOk = true;
    checkOk = true;
}

static void saveBlock(int block, const QImage *src, QList<int> &dest)
{
    int blockX = block % (src->width()/8);
    int blockY = block / (src->width()/8);
    int x = blockX * 8;
    int y = blockY * 8;
    for (int dx=0; dx<8; ++dx)
        for (int dy=0; dy<8; ++dy)
            dest.push_back( src->pixel(x+dx, y+dy) );
}

void DecodrState::savePixels(const DecodrState &prevFrame)
{
    savedPixels.blockBegin = prevFrame.cursor.currentBlockIndex();
    savedPixels.pixels.clear();
    for (int b=prevFrame.cursor.currentBlockIndex(); b<cursor.currentBlockIndex(); ++b)
        saveBlock(b, cursor.canvas(), savedPixels.pixels);
}

bool DecodrState::operator ==(const DecodrState &other) const
{
    return
               lastWasFF    == other.lastWasFF
            && buffer       == other.buffer
            && bufferPos    == other.bufferPos
            && bytesRead    == other.bytesRead
            && pjpegCtxt    == other.pjpegCtxt
//            && imgInfo      == other.imgInfo
//            && cursor       == other.cursor
            && decodOk      == other.decodOk
            && checkOk      == other.checkOk
            && savedPixels.blockBegin   == other.savedPixels.blockBegin
            && savedPixels.pixels       == other.savedPixels.pixels
                    ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Thread : public QThread
{
public:
    Thread(std::function<void()> f_) : f(f_) {}
private:
    void run() { f(); }
    std::function<void()> f;
};

struct PicoJpegDecodr::Private
{
    Private(ICheck *checkr);

    ~Private();

    ICheck *checkr;
    QImage image;
//    PicoJpegDecodr::State state;
    DecodrState state;

    int blockCount;
    bool end;
    bool inited;

    QThread *thread;
    QMutex mutex;
    QWaitCondition cond;

    Common::Cluster inCluster;
    enum Status { Init, Decode, Exiting, Idle };
    Status status;

    void shiftBuffers();
    bool innerBufferEnough() const  { return (state.buffer.size() - state.bufferPos) >= pjpeg_max_in_buf_size; }
    bool innerBufferEmpty() const   { return state.buffer.size() == state.bufferPos; }
    bool outerBufferEmpty() const   { return inCluster.second.size() == 0; }
    bool checkFFD9() const;
    int latestBlock() const;   

    void threadFunc();

    void processInit();
    void processDecode(bool makeCheck=true);

    static unsigned char fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param);
    unsigned char fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read);
};

PicoJpegDecodr::PicoJpegDecodr(ICheck *checkr)
{
    m_d = new Private(checkr);
}

PicoJpegDecodr::~PicoJpegDecodr()
{
    delete m_d;
}

PicoJpegDecodr::Private::Private(ICheck *checkr)
: checkr(checkr)
, image()
, state(&image)
, blockCount(0)
, end(false)
, inited(false)
, thread(new Thread(std::bind(&PicoJpegDecodr::Private::threadFunc, this)) )
, status( Idle )
{
    mutex.lock();
}

void PicoJpegDecodr::init()
{
    qDebug()<<__FUNCTION__;

    m_d->inited = false;

    m_d->thread->start();
    // wait until thread is started
    m_d->cond.wait( &m_d->mutex );
}

bool PicoJpegDecodr::feed(const Common::Cluster &cluster)
{
    qDebug()<<__FUNCTION__<<cluster.first;

    if (m_d->status == Private::Idle)
        m_d->status = m_d->inited ? Private::Decode : Private::Init;

    m_d->inCluster = cluster;
    m_d->cond.wakeAll();
    m_d->cond.wait( &m_d->mutex );
    return m_d->status == Private::Idle;
}

PicoJpegDecodr::Private::~Private()
{
    status = Exiting;
    mutex.unlock();
    cond.wakeAll();

    thread->wait();
}

void PicoJpegDecodr::Private::threadFunc()
{
    QMutexLocker l( &mutex );

    for (;;) {
        cond.wakeAll();
        cond.wait( &mutex );

        switch (status) {
        case Init:
            processInit();
            processDecode(false);
            break;
        case Decode:
            processDecode();
            break;
        case Exiting:
            return;
        }
        status = Idle;
    }
}

void PicoJpegDecodr::Private::processInit()
{
    qDebug()<<__FUNCTION__;

    uchar rv = pjpeg_decode_init(&state.imgInfo, &PicoJpegDecodr::Private::fetchCallback, this, 0);
    if (rv != 0) {
        Msg("PicoJpegDecodr:init picojpeg err: %d\n", rv);
        end = true;
        state.decodOk = false;
    } else {

        qDebug("INITED!!!!  %d %d", state.imgInfo.m_width, state.imgInfo.m_height);

        state.cursor.initCanvas( QSize(state.imgInfo.m_width, state.imgInfo.m_height) );
        end = false;
        inited = true;
    }
}

void PicoJpegDecodr::Private::processDecode(bool makeCheck)
{
    qDebug()<<__FUNCTION__;

    static const int MAX_BLOCKS_PER_CLUSTER = 450; // empirical

    while (!outerBufferEmpty()
           || innerBufferEnough()) {
        uchar rv = pjpeg_decode_mcu();
        if (rv == 0) {

            for (int i=0; i<2; ++i, ++blockCount) { //< copy pixels
                state.cursor.addBlock(  state.imgInfo.m_pMCUBufR +i*64,
                                        state.imgInfo.m_pMCUBufG +i*64,
                                        state.imgInfo.m_pMCUBufB +i*64  );
            }

            if (state.cursor.atEnd() && checkFFD9()) {
                Msg("END+FFD9");
                end = true;
                break;
            }

            if (blockCount > MAX_BLOCKS_PER_CLUSTER) {
                Msg("[BlockCount overflow %d]", blockCount);
                state.decodOk = false;
                break;
            }

        } else if (rv == PJPG_NO_MORE_BLOCKS) {
            Msg("[PJPG_NO_MORE_BLOCKS]");
            Q_ASSERT(false);
        } else {
            Msg("[Decod Err-%d]", rv);
            state.decodOk = false;
            break;
        }

    }

    Msg("[Decod Ok]");
    if (makeCheck && state.decodOk) {
        state.checkOk = checkr->check(image, latestBlock()-blockCount, latestBlock());
        Msg("[Check %s]", state.checkOk ? "Ok" : "Fail");
    }
}

bool PicoJpegDecodr::Private::checkFFD9() const
{
    if (state.bufferPos == state.bytesRead
        && state.lastWasFF
        && state.buffer[0] == (char)0xD9)
                                            return true;

    for (int i=state.bufferPos - state.bytesRead; i<state.bufferPos; ++i) // find D9
        if (state.buffer[i] == (char)0xD9) // check FF
            if (i != 0 && state.buffer[i-1] == (char)0xFF)
                return true;

    return false;
}

int PicoJpegDecodr::Private::latestBlock() const
{
    return state.cursor.currentBlockIndex() -1;
}


void PicoJpegDecodr::Private::shiftBuffers()
{
    Q_ASSERT(innerBufferEmpty());
    Q_ASSERT(!outerBufferEmpty());

    state.lastWasFF = state.buffer.isEmpty() ? false : state.buffer.endsWith( (char)0xFF );

    state.buffer = inCluster.second;
    inCluster.second.clear();
    state.bufferPos = 0;
    blockCount = 0;
}

unsigned char PicoJpegDecodr::Private::fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr::Private *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::Private::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
        qDebug()<<__FUNCTION__<<buf_size<<(status==Init ? "INIT":"DECOD");

    if (status == Init) {
        while (innerBufferEmpty()) {

            qDebug()<<"EMPTY. Wait for feed";

            cond.wakeAll();
            cond.wait( &mutex );
            if (status != Init) {
                qDebug("EIXT NO_MORE+_BLOCKS");
                return PJPG_NO_MORE_BLOCKS;
            }
            if (!outerBufferEmpty())
                shiftBuffers();

            qDebug("GOT IT");
        }


    } else {
        // fetch
        if (innerBufferEmpty()) {
            if (outerBufferEmpty())
                return PJPG_NO_MORE_BLOCKS;
            shiftBuffers();
        }
    }

    state.bytesRead = *pBytes_actually_read = qMin( state.buffer.size() - state.bufferPos, (int)buf_size );
    memcpy( pBuf, state.buffer.data() + state.bufferPos, *pBytes_actually_read );
    state.bufferPos += *pBytes_actually_read;

    return 0;
}

bool PicoJpegDecodr::initialized() const
{
    return m_d->inited;
}

bool PicoJpegDecodr::checkOk() const
{
    return m_d->state.checkOk;
}

bool PicoJpegDecodr::decodOk() const
{
    return m_d->state.decodOk;
}

bool PicoJpegDecodr::end() const
{
    return m_d->end;
}

const PicoJpegDecodr::State &PicoJpegDecodr::state() const
{
        qDebug()<<__FUNCTION__;

    pjpeg_save_ctxt( m_d->state.pjpegCtxt.data() );
    return m_d->state;
}

void PicoJpegDecodr::doRestore(const PicoJpegDecodr::State &state)
{
    qDebug()<<__FUNCTION__;

    m_d->state = state;
    pjpeg_load_ctxt( m_d->state.pjpegCtxt.data() );
}




































#if 0

int PicoJpegDecodFrame::id_gen = 1;

PicoJpegDecodFrame::PicoJpegDecodFrame(QImage *image)
: DecodrFrame(id_gen++, JpegContextType)
{
    pjpegCtxt.resize(pjpeg_ctxt_buffer_size);

    bufferPos = 0;
    bytesRead = 0;
    buffer.clear();
    lastWasFF = false;
    if (image) {
        cursor.setCanvas(image);
        cursor.restart();
    }
    decodeOkValue = true;
}

PicoJpegDecodFrame *PicoJpegDecodFrame::clone() const
{
    return new PicoJpegDecodFrame( *this );
}

static void saveBlock(int block, const QImage *src, QList<int> &dest)
{
    int blockX = block % (src->width()/8);
    int blockY = block / (src->width()/8);
    int x = blockX * 8;
    int y = blockY * 8;
    for (int dx=0; dx<8; ++dx)
        for (int dy=0; dy<8; ++dy)
            dest.push_back( src->pixel(x+dx, y+dy) );
}

void PicoJpegDecodFrame::savePixels(const PicoJpegDecodFrame &prevFrame)
{
    savedPixels.blockBegin = prevFrame.cursor.currentBlockIndex();
    savedPixels.pixels.clear();
    for (int b=prevFrame.cursor.currentBlockIndex(); b<cursor.currentBlockIndex(); ++b) 
        saveBlock(b, cursor.canvas(), savedPixels.pixels);    
}

bool PicoJpegDecodFrame::decodeOk() const
{
    return decodeOkValue;
}

///////////////////////////////////

PicoJpegDecodr::PicoJpegDecodr(ICheck *check, QObject *parent)
: Decodr(parent)
{
    mCheck = check;
    mFetch = nullptr;
    mDone = false;
    mBlockCount = 0;
    mWasFetched = 0;
}


bool PicoJpegDecodr::restart(Fetch *fetch)
{
    mFetch = fetch;

    // reset context
    mFrame = PicoJpegDecodFrame(&mImage);

    uchar rv = pjpeg_decode_init(&mFrame.imgInfo, &PicoJpegDecodr::fetchCallback, this, 0);
    if (rv != 0) {
        Msg("PicoJpegDecodr::restart() picojpeg err: %d\n", rv);
        mDone = true;
        mFrame.setDecodeFailed();
        emit rejected(mFrame);
        return false;

    } else {
        mFrame.cursor.initCanvas(QSize(mFrame.imgInfo.m_width, mFrame.imgInfo.m_height));
        mDone = false;
        emit accepted(mFrame);
        return true;
    }
}

void PicoJpegDecodr::resume()
{
    PicoJpegDecodFrame prevFrame;

    qDebug("RESUME");
    while ( !mFetch->atEnd() && !isDone()) {
        saveFrame(prevFrame);

        if (decodeCluster()) {
            emit accepted(mFrame);

        } else {
            if (mFrame.decodeOk()) //< if parsing was successful, then save the fragment of image
                mFrame.savePixels( prevFrame );
            emit rejected(mFrame);
            loadFrame(prevFrame);
        }
    }

    if (isDone()) {
        Msg("\nEMIT DONE");
        emit done();
    }        
}

bool PicoJpegDecodr::decodeCluster()
{
//    qDebug("DECODE_CLUSTER");
    bool retval = true;
    mWasFetched = false;
    uchar rv = 0;
    for (rv=pjpeg_decode_mcu(); rv==0; rv=pjpeg_decode_mcu()) {

        for (int i=0; i<2; ++i, ++mBlockCount) { //< copy pixels
            mFrame.cursor.addBlock( mFrame.imgInfo.m_pMCUBufR +i*64,
                                    mFrame.imgInfo.m_pMCUBufG +i*64,
                                    mFrame.imgInfo.m_pMCUBufB +i*64  );
        }

        if (mFrame.cursor.atEnd() && checkFFD9()) {
            Msg("END+FFD9");
            mDone = true;
            return true;
        }


        if (mWasFetched && mBlockCount > 450) {
            Msg("[BlockCount too big %d]", mBlockCount);
//            mFrame.setDecodeFailed();            
            retval = false;
            break;
        }

        if (mWasFetched && ((mFrame.buffer.size() - mFrame.bufferPos)<252) ) {
            Msg("[Decod Ok]");

            bool ch = mCheck->check(mImage, latestBlock() - mBlockCount, latestBlock());
            Msg("[Check-%s]", ch ? "OK":"Fail");
            retval = ch;
            break;
        }

    }


    // processing rv != 0
    if (rv == PJPG_NO_MORE_BLOCKS) {
        Msg("[PJPG_NO_MORE_BLOCKS]");
    } else if (rv > 0) {
        Msg("[Error in Decoding %d]", rv);
        mFrame.setDecodeFailed();        
        retval = false;
    }

//    qDebug("DECOD_CLUSTER EXIT");
    return retval;
}

void PicoJpegDecodr::saveFrame(PicoJpegDecodFrame &outFrame)
{
    pjpeg_save_ctxt(mFrame.pjpegCtxt.data());
    outFrame = mFrame;
}

void PicoJpegDecodr::loadFrame(const DecodrFrame &frame)
{
//    qDebug("LOAD_FRAME");
    if (frame.type() == PicoJpegDecodFrame::JpegContextType) {
        mFrame = static_cast<const PicoJpegDecodFrame &>(frame);
        pjpeg_load_ctxt(mFrame.pjpegCtxt.data());

    } else
        Msg("loadFrame error. frame.type != PicoJpegDecodFrame\n");    
}

bool PicoJpegDecodr::checkFFD9() const
{
    if (mFrame.bufferPos == mFrame.bytesRead
        && mFrame.lastWasFF
        && mFrame.buffer[0] == (char)0xD9)      
                                            return true;

    for (int i=mFrame.bufferPos - mFrame.bytesRead; i<mFrame.bufferPos; ++i) // find D9
        if (mFrame.buffer[i] == (char)0xD9) // check FF
            if (i != 0 && mFrame.buffer[i-1] == (char)0xFF)
                return true;

    return false;
}

bool PicoJpegDecodr::isDone() const
{
    return mDone;
}

int PicoJpegDecodr::latestBlock() const
{
    return mFrame.cursor.currentBlockIndex() -1;
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
//    Msg("FETCH %d %d\n", buf_size, mContext.buffer.size() - mContext.bufferPos);

    if (mFrame.bufferPos == mFrame.buffer.size()) {
        // call fetch.
        mFrame.lastWasFF = mFrame.buffer.isEmpty() ? false : mFrame.buffer.endsWith( (char)0xFF );

        int clusterNo;
        mFetch->fetch(clusterNo, mFrame.buffer);
        if (mFrame.buffer.size() > 0) {
            mFrame.bufferPos = 0;
            mBlockCount = 0;
            mWasFetched = true;

        } else 
            return PJPG_NO_MORE_BLOCKS;
    }

    // copy bytes to picojpeg buffer
    mFrame.bytesRead = *pBytes_actually_read = qMin( mFrame.buffer.size() - mFrame.bufferPos, (int)buf_size);
    memcpy( pBuf, mFrame.buffer.data() + mFrame.bufferPos, *pBytes_actually_read);
    mFrame.bufferPos += *pBytes_actually_read;

    return 0;
}


#endif

} // eons Jpeg
