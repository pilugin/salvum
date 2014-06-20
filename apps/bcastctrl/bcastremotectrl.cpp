#include "bcastremotectrl.h"

using namespace IPCFetch;

BcastRemoteCtrl::BcastRemoteCtrl(const char *shmem)
: mData( SharedBcastCtrlData::attach(shmem) )
{
}

BcastRemoteCtrl::~BcastRemoteCtrl()
{
    if (mData)
        SharedBcastCtrlData::detach(mData);
}

bool BcastRemoteCtrl::acquire()
{
    mData->mutex().lock();

    while ( ! mData->isFree() )
        mData->cond().wait( mData->mutex() );

    return true;
}

bool BcastRemoteCtrl::exchange()
{
    int id = mData->setId();
    mData->msgType = Request;

    while ( ! mData->isResponse(id) ) {
        mData->cond().broadcast();
        mData->cond().timedWait( mData->mutex(), 1000 );
    }

    return true;
}

bool BcastRemoteCtrl::free()
{
    mData->free();

    mData->cond().broadcast();
    mData->mutex().unlock();

    return true;
}

