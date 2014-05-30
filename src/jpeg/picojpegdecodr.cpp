#include "jpeg/picojpegdecodr.h"
#include "if/ifetch.h"
#include "if/iresults.h"
#include "if/ilog.h"
#include "jpeg/advancedchecker.h"

#include <QtDebug>

using namespace Log;

PicoJpegDecodr::PicoJpegDecodr()
{    
    mCheck = new AdvancedChecker;
    mFetch = nullptr;
    mDone = false;
    mBlockCount = 0;
    mWasFetched = 0;
}


PicoJpegDecodr::~PicoJpegDecodr()
{
    delete mCheck;
}

bool PicoJpegDecodr::restart(IFetch *fetch)
{
    mFetch = fetch;

    mContext.bufferPos = 0;
    mContext.buffer.clear();

    mContext.cursor.setCanvas(&mImage);
    mContext.cursor.restart();

    mUsedClusters.clear();
    uchar rv = pjpeg_decode_init(&mContext.imgInfo, &PicoJpegDecodr::fetchCallback, this, 0);
    if (rv != 0) 
        Msg("PicoJpegDecodr::restart() picojpeg err: %d\n", rv);

    mContext.cursor.initCanvas(QSize(mContext.imgInfo.m_width, mContext.imgInfo.m_height));
    mDone = false;

    return rv ==0;
}

bool PicoJpegDecodr::decodeCluster()
{
    mBackupContext = save();
    mUsedClusters.clear();

    bool retval = true;
    mWasFetched = false;
    uchar rv = 0;
    for (rv=pjpeg_decode_mcu(); rv==0; rv=pjpeg_decode_mcu()) {

        for (int i=0; i<2; ++i, ++mBlockCount) { //< copy pixels
            mContext.cursor.addBlock(   mContext.imgInfo.m_pMCUBufR +i*64,
                                        mContext.imgInfo.m_pMCUBufG +i*64,
                                        mContext.imgInfo.m_pMCUBufB +i*64);
        }

        if (mContext.cursor.atEnd() && checkFFD9()) {
            Msg("END+FFD9");
            mDone = true;
            return true;
        }


        if (mWasFetched && mBlockCount > 450) {
            Msg("[BlockCount too big %d]", mBlockCount);
            retval = false;
            break;
        }

        if (mWasFetched && ((mContext.buffer.size() - mContext.bufferPos)<252) ) {
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

    return retval;
}

void PicoJpegDecodr::revert()
{
    restore(mBackupContext);
}








PicoJpegDecodContext PicoJpegDecodr::save() const
{
    PicoJpegDecodContext r = mContext;
    pjpeg_save_ctxt(r.pjpegCtxt.data());
    return r;
}

void PicoJpegDecodr::restore(const PicoJpegDecodContext &ctxt)
{
    mContext = ctxt;
    pjpeg_load_ctxt(ctxt.pjpegCtxt.data());
}

bool PicoJpegDecodr::checkFFD9() const
{
    if (mContext.bufferPos == mContext.bytesRead && mContext.lastWasFF && mContext.buffer[0] == (char)0xD9)
        return true;

    for (int i=mContext.bufferPos - mContext.bytesRead; i<mContext.bufferPos; ++i) { // find D9
        if (mContext.buffer[i] == (char)0xD9) { // check FF
            if (i != 0 && mContext.buffer[i-1] == (char)0xFF)
                return true;
        }
    }

    return false;
}

bool PicoJpegDecodr::done() const
{
    return mDone;
}

int PicoJpegDecodr::latestBlock() const
{
    return mContext.cursor.currentBlockIndex() -1;
}

QImage &PicoJpegDecodr::image()
{
    return mImage;
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
//    Msg("FETCH %d %d\n", buf_size, mContext.buffer.size() - mContext.bufferPos);

    if (mContext.bufferPos == mContext.buffer.size()) {
        mContext.lastWasFF = mContext.buffer.size() ? mContext.buffer[0] == (char)0xFF : false;

        int clusterNo;
        mFetch->fetch(clusterNo, mContext.buffer);
        if (mContext.buffer.size() > 0) {
            mContext.bufferPos = 0;
            mUsedClusters.push_back(clusterNo);

            mBlockCount = 0;
            mWasFetched = true;

        } else 
            return PJPG_NO_MORE_BLOCKS;
    }

    mContext.bytesRead = *pBytes_actually_read = qMin( mContext.buffer.size() - mContext.bufferPos, (int)buf_size);
    memcpy( pBuf, mContext.buffer.data() + mContext.bufferPos, *pBytes_actually_read);
    mContext.bufferPos += *pBytes_actually_read;

    return 0;
}



PicoJpegDecodContext::PicoJpegDecodContext()
{
    pjpegCtxt.resize(pjpeg_ctxt_buffer_size);
}

