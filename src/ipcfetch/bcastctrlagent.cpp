#include <ipcfetch/bcastctrlagent.h>

using namespace IPC;

namespace IPCFetch {

BcastCtrlAgent::BcastCtrlAgent(const char *shmem)
: mShmemName(shmem)
, mData(SharedBcastCtrlData::create(shmem))
{
    mData->mutex().lock();
}

BcastCtrlAgent::~BcastCtrlAgent()
{
    mData->mutex().unlock();

    SharedBcastCtrlData::destroy(mData, mShmemName.c_str());
}

bool BcastCtrlAgent::waitForRequest()
{
    static const int to = 3*1000;
    
    while (! mData->isRequest())
        if (! mData->cond().timedWait( mData->mutex(), to )) 
            return false;

    return true;
}

void BcastCtrlAgent::sendResponse()
{
    mData->msgType = Response;
    mData->cond().broadcast();
}

} // ns IPCFetch
