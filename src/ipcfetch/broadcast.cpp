#include "broadcast.h"
#include <QByteArray>

using namespace IPC;
using namespace RdWr;

namespace IPCFetch {

Broadcast::Broadcast(const char *shmemName, const char *shmemNameFeedback, IFetch *fetch) 
: Writer< BroadcastMessage >(shmemName)
, mFeedbackName(shmemNameFeedback)
, mFeedback(nullptr)
, mFetch(fetch)
{
    mFeedback = SharedFeedback::create( shmemNameFeedback );
    mFeedback->completeCount = 0;
    mFeedback->feedbackNeeded = false;
}

Broadcast::~Broadcast()
{
    if (mFeedback)
        SharedFeedback::destroy(mFeedback, mFeedbackName.c_str());
}

void Broadcast::write()
{
    mFetch->rewind(0);
    mFetch->fastfwd();

    Writer< BroadcastMessage > ::write();
}

bool Broadcast::prepare(BroadcastMessage &message)
{
    if ( mFetch->atEnd() ) {
        message.status = AtEnd;
        message.clusters.clear();

    } else {
        message.clusters.clear();
        
        int fetchClusterNo;
        QByteArray fetchCluster;
        while (!message.clusters.full()  && !mFetch->atEnd() ) {

            mFetch->fetch(fetchClusterNo, fetchCluster);
            if (fetchClusterNo == IFetch::InvalidClusterNo || fetchCluster.size() == 0)
                break;

            message.clusters.push_back_empty();
            message.clusters.back().cluster.set( fetchCluster.data(), fetchCluster.size() );
            message.clusters.back().clusterNo = fetchClusterNo;
        }

        message.status = (message.clusters.size() > 0) ? InProgress : AtEnd;

    }

    return true;
}

void Broadcast::postRead(const BroadcastMessage &message)
{
    static const int feedbackTimeout = 2*1000;

    if (message.status == AtEnd) {

        // check feedback here
        // lock until completeCount == regCount -OR- timeout
        Mutexes<1> &m   = mFeedback->mutexes();
        Conds<2> &c     = mFeedback->conds();

        m.mutex().lock();
        mFeedback->feedbackNeeded = true;
        while ( mFeedback->completeCount < sharedMem().regCount ) {
            c.cond<RECV>().broadcast();
            if ( ! c.cond<BCAST>().timedWait(m.mutex(), feedbackTimeout) ) //< timeout
                break;

            // process feedback partly
            QVector<int> tmpvector( mFeedback->skip.size() );
            memcpy( tmpvector.data(), mFeedback->skip.data(), sizeof(int) * mFeedback->skip.size() );
            mFetch->skip( tmpvector );
            mFeedback->skip.clear();
        }
        mFeedback->completeCount = 0;        
        mFeedback->feedbackNeeded = false;
        m.mutex().unlock();        

        mFetch->rewind(0);
        mFetch->fastfwd();
    }
}

bool Broadcast::isValid() const
{
    return mFeedback != nullptr && Writer<BroadcastMessage>::isValid();
}

} // ns IPCFetch
