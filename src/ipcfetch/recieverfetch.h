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

class RecieverFetch : public Fetch, public RdWr::Reader<BroadcastMessage>
{
public:
    RecieverFetch(const char *shmemName, QObject *parent =nullptr);
    ~RecieverFetch();

    bool rewind(int clusterNo);
    void skip(int clusterNo, int length);
    void fastfwd();
    bool atEnd() const;

    bool isValid() const;
    
protected:
    void doFetch(int &clusterNo, QByteArray &cluster);
    bool process(const BroadcastMessage &msg);
    void duringReg();
    void duringUnreg();

private:
    SharedFeedback                  *mFeedback;
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
