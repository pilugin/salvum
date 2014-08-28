#include "jpeg/picojpegdecodr.h"
#include "if/ifetch.h"
#include "if/iresults.h"
#include "if/ilog.h"
#include "if/jpeg/icheck.h"

#include <QtDebug>

using namespace Log;

namespace Jpeg {

PicoJpegDecodr::PicoJpegDecodr(ICheck *check)
{    
    mCheck = check;
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

    // reset history & context
    mHistory.clear();
    
    mPjpgContext.bufferPos = 0;
    mPjpgContext.bytesRead = 0;
    mPjpgContext.buffer.clear();
    mPjpgContext.lastWasFF = false;
    mPjpgContext.cursor.setCanvas(&mImage);
    mPjpgContext.cursor.restart();

    uchar rv = pjpeg_decode_init(&mPjpgContext.imgInfo, &PicoJpegDecodr::fetchCallback, this, 0);
    if (rv != 0) 
        Msg("PicoJpegDecodr::restart() picojpeg err: %d\n", rv);

    mPjpgContext.cursor.initCanvas(QSize(mPjpgContext.imgInfo.m_width, mPjpgContext.imgInfo.m_height));
    mDone = false;

    return rv == 0;
}

bool PicoJpegDecodr::decodeCluster()
{
    savePjpgContext();

    bool retval = true;
    mWasFetched = false;
    uchar rv = 0;
    for (rv=pjpeg_decode_mcu(); rv==0; rv=pjpeg_decode_mcu()) {

        for (int i=0; i<2; ++i, ++mBlockCount) { //< copy pixels
            mPjpgContext.cursor.addBlock(   mPjpgContext.imgInfo.m_pMCUBufR +i*64,
                                            mPjpgContext.imgInfo.m_pMCUBufG +i*64,
                                            mPjpgContext.imgInfo.m_pMCUBufB +i*64  );
        }

        if (mPjpgContext.cursor.atEnd() && checkFFD9()) {
            Msg("END+FFD9");
            mDone = true;
            return true;
        }


        if (mWasFetched && mBlockCount > 450) {
            Msg("[BlockCount too big %d]", mBlockCount);
            retval = false;
            break;
        }

        if (mWasFetched && ((mPjpgContext.buffer.size() - mPjpgContext.bufferPos)<252) ) {
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

void PicoJpegDecodr::revert(int frameNo)
{
    while (frameNo--> 0 && mHistory.size() > 1)
        mHistory.pop();
    mPjpgContext = mHistory.pop().pjpgContext;
}






void PicoJpegDecodr::savePjpgContext()
{
    pjpeg_save_ctxt(mPjpgContext.pjpegCtxt.data());
    mHistory.push(JpegContext(mPjpgContext));
}

void PicoJpegDecodr::addClusterToHistory(int clusterNo)
{    
    mHistory.top().clusterNos.push_back(clusterNo);
}



bool PicoJpegDecodr::checkFFD9() const
{
    if (mPjpgContext.bufferPos == mPjpgContext.bytesRead
        && mPjpgContext.lastWasFF
        && mPjpgContext.buffer[0] == (char)0xD9)
        return true;

    for (int i=mPjpgContext.bufferPos - mPjpgContext.bytesRead; i<mPjpgContext.bufferPos; ++i) { // find D9
        if (mPjpgContext.buffer[i] == (char)0xD9) { // check FF
            if (i != 0 && mPjpgContext.buffer[i-1] == (char)0xFF)
                return true;
        }
    }

    return false;
}

bool PicoJpegDecodr::done() const
{
    return mDone;
}

const IDecod::Context *PicoJpegDecodr::historyFrame(int frameNo) const
{
    return &mHistory[mHistory.size() -1 -frameNo];
}

int PicoJpegDecodr::latestBlock() const
{
    return mPjpgContext.cursor.currentBlockIndex() -1;
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
//    Msg("FETCH %d %d\n", buf_size, mContext.buffer.size() - mContext.bufferPos);

    if (mPjpgContext.bufferPos == mPjpgContext.buffer.size()) {
        // call fetch.
        mPjpgContext.lastWasFF = mPjpgContext.buffer.size() ? mPjpgContext.buffer[ mPjpgContext.buffer.size()-1 ] == (char)0xFF : false;

        int clusterNo;
        mFetch->fetch(clusterNo, mPjpgContext.buffer);
        if (mPjpgContext.buffer.size() > 0) {
            mPjpgContext.bufferPos = 0;
            addClusterToHistory(clusterNo);

            mBlockCount = 0;
            mWasFetched = true;

        } else 
            return PJPG_NO_MORE_BLOCKS;
    }

    mPjpgContext.bytesRead = *pBytes_actually_read = qMin( mPjpgContext.buffer.size() - mPjpgContext.bufferPos, (int)buf_size);
    memcpy( pBuf, mPjpgContext.buffer.data() + mPjpgContext.bufferPos, *pBytes_actually_read);
    mPjpgContext.bufferPos += *pBytes_actually_read;

    return 0;
}



PicoJpegDecodContext::PicoJpegDecodContext()
{
    pjpegCtxt.resize(pjpeg_ctxt_buffer_size);
}

PicoJpegDecodr::JpegContext::JpegContext(const PicoJpegDecodContext &pjpgContext_)
    : pjpgContext(pjpgContext_)
{
}

} // eons Jpeg
