#include <jpeg/picojpegdecodr.h>
#include <core/fetch.h>
#include <util/ilog.h>
#include <jpeg/icheck.h>

#include <QtDebug>

using namespace Log;
using namespace Core;

namespace Jpeg {

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
}

PicoJpegDecodFrame *PicoJpegDecodFrame::clone() const
{
    return new PicoJpegDecodFrame( *this );
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
            emit rejected(mFrame);
            loadFrame(prevFrame);
        }
    }

    if (isDone())
        emit done();
}

bool PicoJpegDecodr::decodeCluster()
{
    qDebug("DECODE_CLUSTER");
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
        retval = false;
    }

    qDebug("DECOD_CLUSTER EXIT");
    return retval;
}

void PicoJpegDecodr::saveFrame(PicoJpegDecodFrame &outFrame)
{
    pjpeg_save_ctxt(mFrame.pjpegCtxt.data());
    outFrame = mFrame;
}

void PicoJpegDecodr::loadFrame(const DecodrFrame &frame)
{
    qDebug("LOAD_FRAME");
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




} // eons Jpeg
