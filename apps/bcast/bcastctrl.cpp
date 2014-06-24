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
    Thread(const char *bcast_shmem, const char *bcastfb_shmem, 
              const QString &deviceFile, const QString &mapFile) 
    : mBcast(0)
    , mBcastShmem(bcast_shmem)
    , mBcastfbShmem(bcastfb_shmem)
    , mDeviceFile(deviceFile)
    , mMapFile(mapFile) 
    {} 

    Broadcast *bcast() { return mBcast; } 
private:
    void run()
    {
        mBcast = new Broadcast(mBcastShmem, mBcastfbShmem, new DeviceMapFetch(mDeviceFile, mMapFile)  );

        mBcast->write();

        delete mBcast;
        mBcast = 0;
    }

    Broadcast *mBcast;
    const char *mBcastShmem;
    const char *mBcastfbShmem;
    QString mDeviceFile;
    QString mMapFile;

};




BcastCtrl::BcastCtrl(const char *bcast_shmem, const char *bcastfb_shmem, const char *ctrl_shmem, 
                        const QString &deviceFile, const QString &mapFile)
: mCtrl(new BcastCtrlAgent(ctrl_shmem))
, mThread(new Thread(bcast_shmem, bcastfb_shmem, deviceFile, mapFile))
{
}

BcastCtrl::~BcastCtrl()
{
    delete mCtrl;
    delete mThread;
}

bool BcastCtrl::isValid() const
{
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
        mThread->bcast()->pause();
        break;

    case Resume:
        qDebug("Resume");
        mThread->bcast()->resume();
        break;

    case Exit:
        qDebug("Exit");
        mThread->bcast()->stop();
        break;

    case Brute:
        qDebug("Brute: %s", rq.boolparam ? "On" : "Off");
//        mFetch->setBrute( rq.boolparam );
        qDebug("Not implemented");
        break;

    case DumpStats:
        qDebug("DumpStats"); 
        {
            QByteArray stats = mThread->bcast()->dumpStats();
            rs.str.set( stats.data() );
        }
        break;

    case SaveBitmap:
        {
            QPair<bool, QString> rv = mThread->bcast()->saveBitmap( rq.strparam.data() );
            rs.success = rv.first;
            rs.str.set( rv.second.toUtf8().data() );
            qDebug("SaveBitmap: %s   %s", rq.strparam.data(), rs.success ? "Ok" : "Failed" );
            if (!rs.success) 
                qDebug("\t%s", rs.str.data() );
        }
        break;
    
    default:
        qDebug("Command unhandled: %d", (int)rq.command);
        rs.success = false;
        QByteArray rstext("unhandled command");
        rs.str.set(rstext.data(), rstext.size());
    }

    return rq.command != Exit;
}


