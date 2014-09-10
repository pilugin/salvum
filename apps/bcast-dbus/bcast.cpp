#include "bcast.h"

Bcast::Bcast(const char *shmem, const SlotClosure &progressCallback, const SlotClosure &bitmapInfoCallback, Fetch *fetch) 
: IPCFetch::Broadcast(shmem, fetch) 
, mProgressCallback(progressCallback)
, mBitmapInfoCallback(bitmapInfoCallback)
{
    mProgress.currentCluster = 0;
    mProgress.clusterCount = 0;//fetch_->bitmap().size();
}

void Bcast::emitProgress()
{
    interrupt(EmitProgress);
}

void Bcast::emitBitmapInfo()
{
    interrupt(EmitBitmapInfo);
}



void Bcast::skip(int clusterNo, int length)
{
    mSkip.clusterNo = clusterNo;
    mSkip.length = length;
    interrupt(Skip);
}

bool Bcast::processInternalMsg(int internalMsg)
{
    switch (internalMsg) {
    case EmitProgress:
        doEmitProgress();
        return true;
    case EmitBitmapInfo:
        doEmitBitmapInfo();
        return true;        
    case Skip:
        fetch()->skip(mSkip.clusterNo, mSkip.length);
        return true;
        
    default:
        return Super::processInternalMsg(internalMsg);
    }
}

void Bcast::doEmitProgress()
{
    qDebug("DO_EMIT_PROGRESS");
    mProgressCallback.call(Q_ARG(int, mProgress.currentCluster), Q_ARG(int, mProgress.clusterCount));
}

void Bcast::doEmitBitmapInfo()
{
    qDebug("DO_EMIT_BITMAP_INFO");
    QList<int> jpegHeads, goodHeads;
    BitmapInfo info;
    bitmapInfo(fetch()->bitmap(), jpegHeads, goodHeads, info);
    mBitmapInfoCallback.call(Q_ARG(QList<int>, jpegHeads), Q_ARG(QList<int>, goodHeads), Q_ARG(BitmapInfo, info) );
}

void Bcast::postRead(const IPCFetch::BroadcastMessage &message)
{
    if (message.status == IPCFetch::AtEnd)
        mProgress.currentCluster = mProgress.clusterCount;
    else if (message.clusters.empty())
        mProgress.currentCluster = 0;
    else
        mProgress.currentCluster = message.clusters.back().clusterNo;
       
    mProgress.clusterCount = fetch()->bitmap().size();

    doEmitProgress();
    Super::postRead(message);
}

void Bcast::bitmapInfo(const QByteArray &bitmap, QList<int> &jpegHeads, QList<int> &goodHeads, BitmapInfo &info)
{
}