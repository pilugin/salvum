#include "bcast.h"
#include "bcastthread.h"

BcastThread::BcastThread(Bcast &bcast, QObject *parent)
: QThread(parent)
, mFetch(nullptr)
, mBcast(bcast) 
{
}

void BcastThread::startBcast(Fetch *fetch)
{
    mFetch = fetch;
    start();
}

void BcastThread::run()
{    
    mBcast.write(mFetch);
}
