#include "bcastctrl.h"
#include <ipcfetch/bcastctrlagent.h>
#include <ipcfetch/broadcast.h>
#include <devicemapfetch.h>
#include <if/ilog.h>

#include <QThread>
#include <QtDebug>

using namespace IPCFetch;

class BcastCtrl::Thread : public QThread
{
public:
    Thread( Broadcast *bcast ) : mBcast(bcast) {}

private:
    void run()
    {
        qDebug("Start Write");
        mBcast->write();
    }

    Broadcast *mBcast;
};




BcastCtrl::BcastCtrl(const char *bcast_shmem, const char *bcastfb_shmem, const char *ctrl_shmem, 
                        const QString &deviceFile, const QString &mapFile)
: mCtrl(new BcastCtrlAgent(ctrl_shmem))
, mBcast(0)
, mFetch(new DeviceMapFetch(deviceFile, mapFile))
, mThread(0)
{
    mBcast = new Broadcast(bcast_shmem, bcastfb_shmem, mFetch);
    mThread = new Thread(mBcast);
}

BcastCtrl::~BcastCtrl()
{
    delete mCtrl;
    delete mBcast;
    // don't delete fetch, as it is deleted by bcast
    delete mThread;
}

bool BcastCtrl::isValid() const
{
    if (! mBcast->isValid() ) {
        qDebug("Broadcast object is invalid");
        return false;
    }

    if (! mCtrl->isValid() ) {
        qDebug("BcastCtrl object is invalid");
        return false;
    }

    return true;
}

bool BcastCtrl::exec()
{
    mThread->start();

    for (;;) {
        if ( mCtrl->waitForRequest() ) {
            bool exit = ! processRq( mCtrl->request(), mCtrl->response() );
            mCtrl->sendResponse();

            if (exit) break;
        }
    }

    mThread->wait();
    return true;
}

bool BcastCtrl::processRq( const BcastCtrlRequest &rq, BcastCtrlResponse &rs )
{
    rs.success = true;
    rs.str.clear();

    switch ( rq.command ) {
    case Noop: break;
    case Pause:
        qDebug("Pause");
        mBcast->pause();
        break;

    case Resume:
        qDebug("Resume");
        mBcast->resume();
        break;

    case Exit:
        qDebug("Exit");
        mBcast->stop();
        break;

    case Brute:
        qDebug("Brute: %s", rq.boolparam ? "On" : "Off");
        mFetch->setBrute( rq.boolparam );
        break;

    case DumpStats:
        qDebug("Stats"); 
        {
            QMap<char, int> stats = mBcast->getMapStats();
//            rs.strparam.data()
        }
        break;

    case SaveBitmap:
        break;
    
    default:
        qDebug("Command unhandled: %d", (int)rq.command);
        rs.success = false;
        QByteArray rstext("unhandled command");
        rs.str.set(rstext.data(), rstext.size());
    }

    return rq.command != Exit;
}


