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
    end = false;
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
            && end          == other.end
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
    DecodrState state;

    int blockCount;
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
, inited(false)
, thread(new Thread(std::bind(&PicoJpegDecodr::Private::threadFunc, this)) )
, status( Idle )
{
    mutex.lock();
}

void PicoJpegDecodr::init()
{
//    qDebug()<<__FUNCTION__;
    m_d->inCluster.first = Common::InvalidClusterNo;
    m_d->inCluster.second.clear();

    m_d->inited = false;

    m_d->thread->start();
    // wait until thread is started
    m_d->cond.wait( &m_d->mutex );
}

bool PicoJpegDecodr::feed(const Common::Cluster &cluster)
{
    qDebug()<<__FUNCTION__<<cluster.first<<m_d->inCluster.first;

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
        state.end = true;
        state.decodOk = false;
    } else {

//        qDebug("INITED!!!!  %d %d", state.imgInfo.m_width, state.imgInfo.m_height);

        state.cursor.initCanvas( QSize(state.imgInfo.m_width, state.imgInfo.m_height) );
        inited = true;
    }
}

void PicoJpegDecodr::Private::processDecode(bool makeCheck)
{
//    qDebug()<<__FUNCTION__;

    static const int MAX_BLOCKS_PER_CLUSTER = 470; // empirical
    blockCount = 0;

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
                state.end = true;
                image.save("SALV/WORK.jpg");
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

    if (state.decodOk) {
        Msg("[Decod Ok]");
        if (makeCheck) {
            state.checkOk = checkr->check(image, latestBlock()-blockCount, latestBlock());
            Msg("[Check %s]", state.checkOk ? "Ok" : "Fail");
        }
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
}

unsigned char PicoJpegDecodr::Private::fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr::Private *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::Private::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
    if (status == Init) {
        while (innerBufferEmpty()) {
            if (!outerBufferEmpty()) {
                shiftBuffers();
                break;
            }

            cond.wakeAll();
            cond.wait( &mutex );
            if (status != Init) {
                return PJPG_NO_MORE_BLOCKS;
            }
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
    return decodOk() && m_d->state.checkOk;
}

bool PicoJpegDecodr::decodOk() const
{
    return m_d->state.decodOk;
}

bool PicoJpegDecodr::end() const
{
    return m_d->state.end;
}

const PicoJpegDecodr::State &PicoJpegDecodr::state() const
{
    pjpeg_save_ctxt( m_d->state.pjpegCtxt.data() );
    return m_d->state;
}

void PicoJpegDecodr::doRestore(const PicoJpegDecodr::State &state_)
{
    m_d->state = state_;
    pjpeg_load_ctxt( m_d->state.pjpegCtxt.data() );
}

} // eons Jpeg
