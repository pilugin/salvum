#ifndef IPCFETCH_BROADCAST_H
#define IPCFETCH_BROADCAST_H

#include <core/fetch.h>
#include <util/ipc.h>
#include <rdwr/writer.h>
#include <ipcfetch/common.h>
#include <string>

#include <QMap>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QPair>

namespace IPCFetch {

class Broadcast : public RdWr::Writer<BroadcastMessage>
{
    typedef RdWr::Writer<BroadcastMessage> Super;
public:
    Broadcast(const char *shmemName, Core::Fetch *fetch =nullptr);
    
    void write(Core::Fetch *fetch =nullptr, int clusterNo =0);

    QByteArray dumpStats();
    QPair<bool, QString> saveBitmap(const QString &filename);
    void rewind(int clusterNo);
protected:   
    enum InternalOperation { 
        DumpStats = Super::Custom,
        SaveBitmap,
        Custom,
        Rewind
    };
    bool processInternalMsg(int internalMsg);

    bool prepare(BroadcastMessage &message);
    void postRead(const BroadcastMessage &message);
    
    Core::Fetch *fetch() { return mFetch; }
private:
    Core::Fetch    *mFetch;

    QByteArray      mStats;
    bool            mSaveBitmapSuccess;
    QString         mSaveBitmapError;
    QString         mSaveBitmapFilename;
    int             mRewind;
};

} // ns IPCFetch

#endif // IPCFETCH_BROADCAST_H
