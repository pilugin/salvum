#include "broadcast.h"
#include <QByteArray>
#include "if/ilog.h"

using namespace Log;
using namespace IPC;
using namespace RdWr;

namespace IPCFetch {

Broadcast::Broadcast(const char *shmemName, const char *shmemNameFeedback, IFetch *fetch) 
: Writer< BroadcastMessage >(shmemName)
, mFeedbackName(shmemNameFeedback)
, mFeedback(nullptr)
, mFetch(fetch)
{
    sharedData().rewind = IFetch::InvalidClusterNo;

    mFeedback = SharedFeedback::create( shmemNameFeedback );
}

Broadcast::~Broadcast()
{
    delete mFetch;
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
    if ( message.rewind != IFetch::InvalidClusterNo && sharedMem().regCount == 1) {
        Msg("BCAST:prepare rewind(%08X)\n", message.rewind );
        mFetch->rewind( message.rewind );
        message.rewind = IFetch::InvalidClusterNo;
    }

    if ( mFetch->atEnd() ) {
        Msg("BCAST:prepare atEnd\n");

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

        Msg("BCAST:prepare status=%s\n", message.status == InProgress ? "InProgress" : "AtEnd" );
    }

    return true;
}



void Broadcast::postRead(const BroadcastMessage &message)
{
    static const int feedbackTimeout = 1*1000;

    // check feedback here
    Mutexes<1> &m   = mFeedback->mutexes();
    Conds<2> &c     = mFeedback->conds();

    QVector<int> tmpvector;
    bool wasFull = false;

    m.mutex().lock();
    while ( ! mFeedback->skip.empty() ) {       
        wasFull = mFeedback->skip.full();   
        tmpvector.resize( mFeedback->skip.size() );
        memcpy( tmpvector.data(), mFeedback->skip.data(), sizeof(int) * mFeedback->skip.size() );
        mFetch->skip( tmpvector );
        mFeedback->skip.clear();

        if (! wasFull)
            break;

        c.cond<RECV>().broadcast();
        while ( mFeedback->skip.empty() )
            if ( ! c.cond<BCAST>().timedWait(m.mutex(), feedbackTimeout) ) //<timeout 
                break;
    }

    m.mutex().unlock();

    if (message.status == AtEnd) {

        Msg("BCAST:postRead[AtEnd] rewind+fastfwd\n");
        mFetch->rewind(0);
        mFetch->fastfwd();
    }
}

bool Broadcast::isValid() const
{
    return mFeedback != nullptr && Writer<BroadcastMessage>::isValid();
}

//////

QByteArray Broadcast::dumpStats()
{
    mStats.clear();
    interrupt(DumpStats);
    return mStats;
}

QPair<bool, QString> Broadcast::saveBitmap(const QString &filename)
{
    interrupt(SaveBitmap);
    return qMakePair(mSaveBitmapSuccess, mSaveBitmapError);
}

} // ns IPCFetch
