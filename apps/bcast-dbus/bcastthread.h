#ifndef BCASTS_THREAD_H
#define BCASTS_THREAD_H

#include <QThread>

namespace Core3 {
class Fetch;
}
class Bcast;

class BcastThread : public QThread
{
public:
    BcastThread(Bcast &bcast, QObject *parent =nullptr); 
    
    void startBcast(int clusterNo, Core3::Fetch *fetch =nullptr);
protected:
    void run();
    
    Core3::Fetch *mFetch;
    int mClusterNo;
    Bcast &mBcast;
};



#endif // BCASTS_THREAD_H
