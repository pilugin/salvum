#include "recieverfetch.h"

using namespace IPC;
using namespace RdWr;

namespace IPCFetch {

RecieverFetch::RecieverFetch(const char *shmemName, const char *shmemFeedbackName)
: Reader<BroadcastMessage>(shmemName)
{
    mFeedback = SharedFeedback::attach(shmemFeedbackName);
}

RecieverFetch::~RecieverFetch()
{
    if (mFeedback)
        SharedFeedback::detach(mFeedback);
}

bool RecieverFetch::isValid() const
{
    return mFeedback != nullptr && Reader<BroadcastMessage>::isValid();
}

bool RecieverFetch::rewind(int clusterNo, int)
{
    mWaitForCluster = clusterNo;
    return true;
}

void RecieverFetch::fastfwd()
{
}

void RecieverFetch::skip(const QVector<int> &clusters)
{
    Mutexes<1> &m   = mFeedback->mutexes();
    Conds<2> &c     = mFeedback->conds();

    m.mutex().lock();
    if (mFeedback->feedbackNeeded) {
        int i=0;
        for (; i<clusters.size(); ++i) {
            while (mFeedback->skip.full()) {
                c.cond<BCAST>().signal();
                c.cond<RECV>().wait( m.mutex() ); 
            }
            
            mFeedback->skip.push_back(clusters[i]);
            
        }
        ++mFeedback->completeCount;
        c.cond<BCAST>().signal();
    }
    m.mutex().unlock();
}

bool RecieverFetch::atEnd() const
{
    return true;
}

void RecieverFetch::fetch(int &clusterNo, QByteArray &cluster)
{
}

bool RecieverFetch::process(const BroadcastMessage &message)
{
    return true;
}

} // ns IPCFetch