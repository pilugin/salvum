#ifndef IPCFETCH_RECIEVERFETCH_H
#define IPCFETCH_RECIEVERFETCH_H

#include <core/fetch.h>
#include <rdwr/reader.h>
#include <ipcfetch/common.h>

#include <QPair>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

namespace IPCFetch {

class RecieverFetch : public Core::Fetch, public RdWr::Reader<BroadcastMessage>
{
public:
    RecieverFetch(QObject *parent =nullptr);
    RecieverFetch(const char *shmemName, QObject *parent =nullptr);
    ~RecieverFetch();
    
    void appendBeginningCluster(int clusterNo, const QByteArray &cluster);

    bool rewind(int clusterNo);
    void skip(int clusterNo, int length);
    void fastfwd();
    bool atEnd() const;

protected:
    void doFetch(int &clusterNo, QByteArray &cluster);
    bool process(const BroadcastMessage &msg);
    void duringReg();
    void duringUnreg();
    
    void setExiting(bool exiting)   { mExiting = exiting; }
    bool exiting() const            { return mExiting; }
    
    /// this routine is called from main thread, while doFetch is waiting. Use it for eventLoop, etc
    /// @return time in milliseconds to wait for
    virtual int waiting4Fetch();

private:
    int                             mWaitForCluster; //< set by rewind()
    bool                            mRegistered;
    bool                            mAtEnd;
    bool                            mExiting;

    QQueue<QPair<int, QByteArray>>  mRecvClusters;

    class RecvThread;
    RecvThread                      *mRecvThread;
    mutable QMutex                  mInternMtx;
    mutable QWaitCondition          mInternCnd;
    
};

} // ns IPCFetch

#endif // IPCFETCH_RECIEVERFETCH_H
