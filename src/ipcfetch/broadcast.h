#ifndef IPCFETCH_BROADCAST_H
#define IPCFETCH_BROADCAST_H

#include <if/ifetch.h>
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
    Broadcast(const char *shmemName, const char *shmemNameFeedback, IFetch *fetch);
    ~Broadcast();
    
    bool isValid() const;       

    void write();

    QByteArray dumpStats();
    QPair<bool, QString> saveBitmap(const QString &filename);
protected:   
    enum InternalOperation { 
        DumpStats = RdWr::Writer<BroadcastMessage>::Custom, 
        SaveBitmap,
        Custom
    };
    bool processInternalMsg(int internalMsg);

    bool prepare(BroadcastMessage &message);
    void postRead(const BroadcastMessage &message);
private:
    const std::string mFeedbackName;
    SharedFeedback  *mFeedback;
    IFetch          *mFetch;

    QByteArray      mStats;
    bool            mSaveBitmapSuccess;
    QString         mSaveBitmapError;
    QString         mSaveBitmapFilename;
};

} // ns IPCFetch

#endif // IPCFETCH_BROADCAST_H
