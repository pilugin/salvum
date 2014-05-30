#ifndef IPCFETCH_BROADCAST_H
#define IPCFETCH_BROADCAST_H

#include <if/ifetch.h>
#include <util/ipc.h>
#include <rdwr/writer.h>
#include <ipcfetch/common.h>
#include <string>

namespace IPCFetch {

class Broadcast : public RdWr::Writer<BroadcastMessage>
{
public:
    Broadcast(const char *shmemName, const char *shmemNameFeedback, IFetch *fetch);
    ~Broadcast();
    
    void write();
    bool isValid() const;

protected:
    bool prepare(BroadcastMessage &message);
    void postRead(const BroadcastMessage &message);
private:
    const std::string mFeedbackName;
    SharedFeedback  *mFeedback;
    IFetch          *mFetch;
};

} // ns IPCFetch

#endif // IPCFETCH_BROADCAST_H