#ifndef IPCFETCH_RECIEVERFETCH_H
#define IPCFETCH_RECIEVERFETCH_H

#include <if/ifetch.h>
#include <rdwr/reader.h>
#include <ipcfetch/common.h>

#include <QPair>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

namespace IPCFetch {

class RecieverFetch : public IFetch, public RdWr::Reader<BroadcastMessage>
{
public:
    RecieverFetch(const char *shmemName, const char *shmemFeedbackName);
    ~RecieverFetch();

    bool rewind(int clusterNo, int clusterNoEnd);
    void skip(const QVector<int> &clusters);
    void fetch(int &clusterNo, QByteArray &cluster);
    void fastfwd();
    
    bool atEnd() const;

    bool isValid() const;   

protected:
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