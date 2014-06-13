#include <ipcfetch/bcastctrlagent.h>

using namespace IPC;

namespace IPCFetch {

BcastCtrlAgent::BcastCtrlAgent(const char *shmem)
: mShmemName(shmem)
, mData(SharedBcastCtrlData::create(shmem))
{
}

BcastCtrlAgent::~BcastCtrlAgent()
{
    if (mData)
        SharedBcastCtrlData::destroy(mData, mShmemName.c_str());
}

void BcastCtrlAgent::process()
{
    Mutexes<1> &m = mData->mutexes();

    if (m.mutex().tryLock()) {


        m.mutex().unlock();
    }
}

} // ns IPCFetch
