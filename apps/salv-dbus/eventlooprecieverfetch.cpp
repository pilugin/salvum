#include "eventlooprecieverfetch.h"
#include <QtCore/QEventLoop>

EventLoopRecieverFetch::EventLoopRecieverFetch(QObject *parent)
: IPCFetch::RecieverFetch(parent)
, mEventLoop(new QEventLoop(this))
{
}

int EventLoopRecieverFetch::waiting4Fetch()
{
    mEventLoop->processEvents();
    if (exiting())
        return -1;
    return 500;
}

