#include "feedbackresults.h"

using namespace IPC;

namespace IPCFetch {

FeedbackResults::FeedbackResults(const char *shmemName, const QString &dir) 
: RangeFileResults(dir)
, mFeedback(SharedFeedback::attach(shmemName))
{
}

FeedbackResults::~FeedbackResults()
{
    if (mFeedback)
        SharedFeedback::detach(mFeedback);
}

bool FeedbackResults::restart(const QString &session)
{
    mFreshClusters.clear();
    return RangeFileResults::restart(session);
}

void FeedbackResults::addClusters(const QVector<int> &clusterNos)
{
    mFreshClusters += clusterNos;
    if (mFreshClusters.size() >= sClustersToSend)
        sendClusters();

    RangeFileResults::addClusters(clusterNos);
}

void FeedbackResults::finilize(bool success)
{
    if (mFreshClusters.size() > 0)
        sendClusters();

    RangeFileResults::finilize(success);
}

void FeedbackResults::sendClusters()
{
    Mutexes<1> &m = mFeedback->mutexes();
    Conds<2> &c = mFeedback->conds();
    m.mutex().lock();

    auto begin = mFreshClusters.begin();
    auto end = mFreshClusters.end();

    while (begin < end) {
        while (mFeedback->skip.full()) {
            c.cond<BCAST>().signal();
            c.cond<RECV>().wait( m.mutex() );
        }
        
        mFeedback->skip.push_back( *begin++ );
    }

    c.cond<BCAST>().signal();
    m.mutex().unlock();

    mFreshClusters.clear();
}

} // ns IPCFetch

