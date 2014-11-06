#include "bcast.h"
#include "bcastthread.h"

using namespace Core3;

BcastThread::BcastThread(Bcast &bcast, QObject *parent)
: QThread(parent)
, mFetch(nullptr)
, mClusterNo(0)
, mBcast(bcast) 
{
}

void BcastThread::startBcast(int clusterNo, Fetch *fetch)
{
    mFetch = fetch;
    mClusterNo = clusterNo;
    start();
}

void BcastThread::run()
{    
    mBcast.write(mFetch, mClusterNo);
}
