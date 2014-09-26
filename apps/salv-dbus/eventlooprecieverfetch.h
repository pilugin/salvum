#ifndef EVENT_LOOP_RECIEVER_FETCH_H
#define EVENT_LOOP_RECIEVER_FETCH_H

#include <ipcfetch/recieverfetch.h>

class QEventLoop;

class EventLoopRecieverFetch : public IPCFetch::RecieverFetch
{
public:
    explicit EventLoopRecieverFetch(QObject *parent);
    
    void exit() { setExiting(true); }
protected:
    int waiting4Fetch();
private:
    QEventLoop *mEventLoop;    
};

#endif