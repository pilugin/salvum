#ifndef BCASTS_THREAD_H
#define BCASTS_THREAD_H

#include <QThread>

namespace Core {
class Fetch;
}
class Bcast;

class BcastThread : public QThread
{
public:
    BcastThread(Bcast &bcast, QObject *parent =nullptr); 
    
    void startBcast(Core::Fetch *fetch =nullptr);
protected:
    void run();
    
    Core::Fetch *mFetch;
    Bcast &mBcast;
};



#endif // BCASTS_THREAD_H