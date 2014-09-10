#ifndef BCASTS_THREAD_H
#define BCASTS_THREAD_H

#include <QThread>

class Fetch;
class Bcast;

class BcastThread : public QThread
{
public:
    BcastThread(Bcast &bcast, QObject *parent =nullptr); 
    
    void startBcast(Fetch *fetch =nullptr);
protected:
    void run();
    
    Fetch *mFetch;
    Bcast &mBcast;
};



#endif // BCASTS_THREAD_H