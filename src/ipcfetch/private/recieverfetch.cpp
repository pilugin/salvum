#include <ipcfetch/recieverfetch.h>
#include <util/ilog.h>
#include <QThread>
#include <QMutexLocker>

using namespace Log;
using namespace IPC;
using namespace RdWr;
using namespace Core3;

namespace IPCFetch {

class RecieverFetch::RecvThread : public QThread
{
public:
    RecvThread(Reader<BroadcastMessage> &reader)
    : mReader(reader)
    {
    }
protected:
    void run()
    {
        qDebug("Bout 2 reg");
        mReader.reg();

        qDebug("Bout 2 read");
        mReader.read();

        qDebug("Bout 2 unreg");
        mReader.unreg();
    }

    Reader<BroadcastMessage> &mReader;
};

RecieverFetch::RecieverFetch()
: Fetch()
, mWaitForCluster(Common::InvalidClusterNo)
, mRegistered(false)
, mAtEnd(true)
, mExiting(false)
, mRecvThread(nullptr)
{
}

RecieverFetch::RecieverFetch(const char *shmemName)
: Fetch()
, mWaitForCluster(Common::InvalidClusterNo)
, mRegistered(false)
, mAtEnd(true)
, mExiting(false)
, mRecvThread(nullptr)
{
    init(shmemName);
}

RecieverFetch::~RecieverFetch()
{
    if (mRecvThread) {
        {
            QMutexLocker l( &mInternMtx );
            mExiting = true;
        }

        mInternCnd.wakeOne(); // in case when RecvThread sleeps on internal condvar
        mRecvThread->wait();
        delete mRecvThread;
        mRecvThread = nullptr;
    }
}

void RecieverFetch::appendBeginningCluster(int clusterNo, const QByteArray &cluster)
{
    QMutexLocker l( &mInternMtx );

    mRecvClusters.enqueue(qMakePair(clusterNo, cluster));
}

void RecieverFetch::rewind(int clusterNo)
{
    QMutexLocker l( &mInternMtx );

    mAtEnd = false;

    Msg("\nRECV:rewind[%08X]", clusterNo);

    if (!mRegistered && !mRecvThread) {
        mRecvThread = new RecvThread(*this);
        mRecvThread->start();
        if (mRecvClusters.size() >0) {
            if (mRecvClusters.head().first != clusterNo) {
                Msg("\nFailed to RecieverFetch::rewind as head of queue does not correspond to requested cluster"
                    "\nqueue=%08X cluster=%08X", mRecvClusters.head().first, clusterNo);
                return ;//false;
            }
            mWaitForCluster = Common::InvalidClusterNo;
        } else
            mWaitForCluster = clusterNo;
    }
    return ;//true;
}

void RecieverFetch::duringReg()
{
    QMutexLocker l( &mInternMtx );

    mRegistered = true;
}

void RecieverFetch::duringUnreg()
{
    QMutexLocker l( &mInternMtx );

    mRegistered = false;
}

void RecieverFetch::fastfwd()
{
}

void RecieverFetch::skipClusters(const QList<int> &)
{
}

bool RecieverFetch::atEnd() const
{
    QMutexLocker l( &mInternMtx );

    return mAtEnd;
}

Common::Cluster RecieverFetch::doFetch()
{
    Common::Cluster cluster;
    Msg("\n");

    QMutexLocker l( &mInternMtx );
    cluster.first = Common::InvalidClusterNo;

    if (mRecvClusters.size() < Clusters::capacity()/4) {
        Msg("q");
        mInternCnd.wakeOne();
    }
    while (mRecvClusters.size() == 0) {
        Msg("z");
        int to = waiting4Fetch();
        if (to < 0) {
            mAtEnd = true;
            mExiting = true;
            return cluster;
        }
        mInternCnd.wait( &mInternMtx, to );
    }

    cluster.first = mRecvClusters.head().first;
    cluster.second = mRecvClusters.head().second;
    mRecvClusters.dequeue();

    if (cluster.first == Common::InvalidClusterNo) {
        cluster.second.clear();
        mAtEnd = true;

        Msg("[AtEnd]");
    } else {
        mAtEnd = false;
        Msg("[%08X]", cluster.first);
    }

    return cluster;
}

int RecieverFetch::waiting4Fetch()
{
    return 1000;
}

bool RecieverFetch::process(const BroadcastMessage &message)
{
    QMutexLocker l( &mInternMtx );
    Msg("\nthr------ RECV:process ");

    if (mExiting)
        return false;

    // if queue not empty wait.
    while (mRecvClusters.size() > message.clusters.capacity()/4 && !mExiting) {
        Msg("Wait..");
        mInternCnd.wait( &mInternMtx );
    }

    // process
    if (message.status == AtEnd || message.clusters.size() == 0) {
        mRecvClusters.enqueue( qMakePair((int)Common::InvalidClusterNo, QByteArray() ) );
        Msg("BCAST:AtEnd");

    } else if (message.status == About2Quit) {
        mRecvClusters.enqueue( qMakePair((int)Common::InvalidClusterNo, QByteArray() ) );
        Msg("BCAST:About2Quit");
        mExiting = true;

    } else if (!mExiting) {

        int i=0;

        // if rewind was called, skip bad clusters
        if (mWaitForCluster != Common::InvalidClusterNo) {
            Msg("skip until %08X. Current=%08X\n", mWaitForCluster, message.clusters[0].clusterNo);

            for (; i<message.clusters.size(); ++i) 
                if (message.clusters[i].clusterNo == mWaitForCluster) {
                    mWaitForCluster = Common::InvalidClusterNo;
                    break;
                }
        }

        Msg("copy %d", message.clusters.size() -i);
        for (; i<message.clusters.size(); ++i) {

            QPair<int, QByteArray> pair( message.clusters[i].clusterNo, QByteArray() );
            int clsize = message.clusters[i].cluster.size();
            pair.second.resize( clsize );
            memcpy( pair.second.data(), message.clusters[i].cluster.data(), clsize );
            mRecvClusters.enqueue( pair );
        }

    }

    mInternCnd.wakeOne();

    return ! mExiting;
}

} // ns IPCFetch
