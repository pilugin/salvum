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

namespace IPCFetch {

class Broadcast : public RdWr::Writer<BroadcastMessage>
{
public:
    Broadcast(const char *shmemName, const char *shmemNameFeedback, IFetch *fetch);
    ~Broadcast();
    
    bool isValid() const;       

    void write();

    void stop();
    void pause();
    void resume();

    enum RunState { Running, Stopping, Stopped, Pausing, Paused };
    RunState runState() const;

    QMap<char, int> getMapStats() const;

protected:   
    bool prepare(BroadcastMessage &message);
    void postRead(const BroadcastMessage &message);
private:
    const std::string mFeedbackName;
    SharedFeedback  *mFeedback;
    IFetch          *mFetch;

    mutable QMutex  mInternalMtx;
    QWaitCondition  mInternalCnd;

    RunState        mRunState;

};

} // ns IPCFetch

#endif // IPCFETCH_BROADCAST_H
