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
    mHistory.push(PicoJpegDecodContext());
    
    context().bufferPos = 0;
    context().buffer.clear();    

    context().cursor.setCanvas(&mImage);
    context().cursor.restart();

    uchar rv = pjpeg_decode_init(&context().imgInfo, &PicoJpegDecodr::fetchCallback, this, 0);
    if (rv != 0) 
        Msg("PicoJpegDecodr::restart() picojpeg err: %d\n", rv);

    context().cursor.initCanvas(QSize(context().imgInfo.m_width, context().imgInfo.m_height));
    mDone = false;

    return rv == 0;
}

bool PicoJpegDecodr::decodeCluster()
{
    saveContext();

    bool retval = true;
    mWasFetched = false;
    uchar rv = 0;
    for (rv=pjpeg_decode_mcu(); rv==0; rv=pjpeg_decode_mcu()) {

        for (int i=0; i<2; ++i, ++mBlockCount) { //< copy pixels
            mContext.cursor.addBlock(   mContext.imgInfo.m_pMCUBufR +i*64,
                                        mContext.imgInfo.m_pMCUBufG +i*64,
                                        mContext.imgInfo.m_pMCUBufB +i*64);
        }

        if (context().cursor.atEnd() && checkFFD9()) {
            Msg("END+FFD9");
            mDone = true;
            return true;
        }


        if (mWasFetched && mBlockCount > 450) {
            Msg("[BlockCount too big %d]", mBlockCount);
            retval = false;
            break;
        }

        if (mWasFetched && ((context().buffer.size() - context().bufferPos)<252) ) {
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
    while ((frameNo-- + 1) > 0 && mHistory.size() > 1)
	mHistory.pop();
}








void PicoJpegDecodr::saveContext()
{
    pjpeg_save_ctxt(context().pjpegCtxt.data());
    mHistory.push( context() );
}

bool PicoJpegDecodr::checkFFD9() const
{
    if (context().bufferPos == context().bytesRead 
    		&& context().lastWasFF 
		&& context().buffer[0] == (char)0xD9)
        return true;

    for (int i=context().bufferPos - context().bytesRead; i<context().bufferPos; ++i) { // find D9
        if (context().buffer[i] == (char)0xD9) { // check FF
            if (i != 0 && context().buffer[i-1] == (char)0xFF)
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
    return context().cursor.currentBlockIndex() -1;
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param)
{
    return static_cast<PicoJpegDecodr *>(param)->fetchCallback(pBuf, buf_size, pBytes_actually_read);
}

unsigned char PicoJpegDecodr::fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read)
{
//    Msg("FETCH %d %d\n", buf_size, mContext.buffer.size() - mContext.bufferPos);

    if (context().bufferPos == context().buffer.size()) { 
	// call fetch.
        context().lastWasFF = context().buffer.size() ? context().buffer[0] == (char)0xFF : false;

        int clusterNo;
        mFetch->fetch(clusterNo, context().buffer);
        if (context().buffer.size() > 0) {
            context().bufferPos = 0;
            context().usedClusters.push_back(clusterNo);

            mBlockCount = 0;
            mWasFetched = true;

        } else 
            return PJPG_NO_MORE_BLOCKS;
    }

    context().bytesRead = *pBytes_actually_read = qMin( context().buffer.size() - context().bufferPos, (int)buf_size);
    memcpy( pBuf, context().buffer.data() + context().bufferPos, *pBytes_actually_read);
    context().bufferPos += *pBytes_actually_read;

    return 0;
}



PicoJpegDecodContext::PicoJpegDecodContext()
{
    pjpegCtxt.resize(pjpeg_ctxt_buffer_size);
    imagePtr = nullptr;
}

} // eons Jpeg
