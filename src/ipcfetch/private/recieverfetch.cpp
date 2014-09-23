#include <ipcfetch/recieverfetch.h>
#include <util/ilog.h>
#include <QThread>
#include <QMutexLocker>

using namespace Log;
using namespace IPC;
using namespace RdWr;
using namespace Core;

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

RecieverFetch::RecieverFetch(QObject *parent)
: Fetch(parent)
, mWaitForCluster(Fetch::InvalidClusterNo)
, mRegistered(false)
, mAtEnd(true)
, mExiting(false)
, mRecvThread(nullptr)
{
}

RecieverFetch::RecieverFetch(const char *shmemName, QObject *parent)
: Fetch(parent)
, mWaitForCluster(Fetch::InvalidClusterNo)
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

bool RecieverFetch::isValid() const
{
    return mFeedback != nullptr && Reader<BroadcastMessage>::isValid();
}

bool RecieverFetch::rewind(int clusterNo)
{
    QMutexLocker l( &mInternMtx );

    mAtEnd = false;

    Msg("\nRECV:rewind[%08X]", clusterNo);

    if (!mRegistered) {
        mRecvThread = new RecvThread(*this);
        mRecvThread->start();
        mWaitForCluster = clusterNo;
    }
    return true;
}

void RecieverFetch::duringReg()
{
    QMutexLocker l( &mInternMtx );

    mRegistered = true;

    if (sharedMem().regCount == 1 && sharedData().rewind == Fetch::InvalidClusterNo) 
        sharedData().rewind = mWaitForCluster;
}

void RecieverFetch::duringUnreg()
{
    QMutexLocker l( &mInternMtx );

    mRegistered = false;
}

void RecieverFetch::fastfwd()
{
}

void RecieverFetch::skip(int /*clusterNo*/, int /*length*/)
{
}

bool RecieverFetch::atEnd() const
{
    QMutexLocker l( &mInternMtx );

    return mAtEnd;
}

void RecieverFetch::doFetch(int &clusterNo, QByteArray &cluster)
{
    Msg("\n");

    QMutexLocker l( &mInternMtx );

    if (mRecvClusters.size() < Clusters::capacity()/4) {
        Msg("q");
        mInternCnd.wakeOne();
    }
    while (mRecvClusters.size() == 0) {
        Msg("z");
        mInternCnd.wait( &mInternMtx ); 
    }

    clusterNo = mRecvClusters.head().first;
    cluster = mRecvClusters.head().second;
    mRecvClusters.dequeue();
    
    if (clusterNo == Fetch::InvalidClusterNo) {
        cluster.clear();
        mAtEnd = true;
        
        Msg("[AtEnd]");
    } else {
        mAtEnd = false;
        Msg("[%08X]", clusterNo);
    }
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
    if (message.status == AtEnd) {     
        mRecvClusters.enqueue( qMakePair((int)Fetch::InvalidClusterNo, QByteArray() ) );
        Msg("BCAST:AtEnd");
        
    } else if (message.status == About2Quit) {
        mRecvClusters.enqueue( qMakePair((int)Fetch::InvalidClusterNo, QByteArray() ) );
        Msg("BCAST:About2Quit");
        mExiting = true;

    } else {

        int i=0;

        // if rewind was called, skip bad clusters
        if (mWaitForCluster != Fetch::InvalidClusterNo) {
            Msg("skip until %08X", mWaitForCluster);
            for (; i<message.clusters.size(); ++i) 
                if (message.clusters[i].clusterNo == mWaitForCluster) {
                    mWaitForCluster = Fetch::InvalidClusterNo;
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
