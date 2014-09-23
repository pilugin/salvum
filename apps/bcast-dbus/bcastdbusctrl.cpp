#include "bcastdbusctrl.h"
#include "bcastthread.h"
#include "bcast.h"

#include <util/slotclosure.h>
#include <util/devicemapfetch.h>

#include <QFile>
#include <unistd.h>

using namespace Common;
using namespace Core;

class BcastDbusCtrl::Private 
{
public:
    Private(const QString &shmem, const SlotClosure &progressCallback, const SlotClosure &bitmapInfoCallback)
    : bcast(shmem.toUtf8().data(), progressCallback, bitmapInfoCallback, &fetch)
    , thread(bcast)
    , shmemPath(shmem)
    {
    }

    DeviceMapFetch  fetch;
    Bcast           bcast;
    BcastThread     thread;
    
    QString         shmemPath;
};

///////////////////////////////////////////////

BcastDbusCtrl::BcastDbusCtrl(QObject *parent)
: QObject(parent),
m_d(nullptr)
{
    QString shmemPath = QString("%1%2").arg(shmemPathPrefix()).arg(getpid());
    m_d = new Private(
        shmemPath,
        SlotClosure(this, SIGNAL(progress(int,int))),
        SlotClosure(this, SIGNAL(bitmapProcessed(QList<int>,QList<int>,Common::BitmapInfo)))
    );
    new BroadcastAdaptor(this);
}

BcastDbusCtrl::~BcastDbusCtrl()
{
    delete m_d;
    m_d = nullptr;
}

bool BcastDbusCtrl::isValid() const
{
    return m_d->bcast.isValid();
}





Result BcastDbusCtrl::setSource(const QString &mediaPath, const QString &bitmapPath)
{
    Result r;
    
    if (m_d->thread.isRunning()) {
        r.errorCode = -2;
        r.error = "Bcast is running";
    } else {
        r.errorCode = m_d->fetch.init(mediaPath, bitmapPath) ? 0 : -1;
        if (!r)
            r.error = "Failed to init Source";
        else
            emitBitmapProcessed();
    }
    emit shmemCreated(m_d->shmemPath);
    
    return r;
}

void BcastDbusCtrl::start()
{
    if (!m_d->thread.isRunning()) {
        m_d->thread.start();
    }
}

void BcastDbusCtrl::skip(int clusterNo, int length)
{
    qDebug()<<"dbus:SKIP -> ";
    
    if (m_d->thread.isRunning())
        m_d->bcast.skip(clusterNo, length);
}

void BcastDbusCtrl::stop()
{
    if (m_d->thread.isRunning()) 
        m_d->bcast.stop();    
}

void BcastDbusCtrl::dumpStats()
{
    if (m_d->thread.isRunning())
        qDebug()<<m_d->bcast.dumpStats();
    else
        qDebug()<<"not running";
}

Result BcastDbusCtrl::saveBitmap(const QString &bitmapPath)
{
    Result r;
    if (m_d->thread.isRunning()) {
        auto rr = m_d->bcast.saveBitmap(bitmapPath);
        r.errorCode = rr.first ? 0 : -1;
        r.error = rr.second;
    } else {
        QByteArray bm = m_d->fetch.bitmap();
        QFile out(bitmapPath);
        if (! out.open(QFile::WriteOnly | QFile::Truncate) ) {
            r.errorCode = -2;
            r.error = out.errorString();

        } else {
            int written = out.write(bm);
            r.errorCode = (written == bm.size()) ? 0 : -3;
            if (!r)
                r.error.sprintf("Could not write whole bitmap file. Only %d/%d written", written, bm.size() );
        }
        
    }
    return r;
}

void BcastDbusCtrl::emitProgress()
{
    qDebug()<<"dbus:EMIT_PROGRESS -> ";
    
    if (m_d->thread.isRunning())
        m_d->bcast.emitProgress();
    else
        emit progress(-1, -1);
}

void BcastDbusCtrl::emitBitmapProcessed()
{
    qDebug()<<"dbus:EMIT_BITMAP_PROCESSED -> ";
    if (m_d->thread.isRunning())
        m_d->bcast.emitBitmapInfo();
    else {
        QList<int> jpegHeads, goodHeads;
        BitmapInfo info;
        Bcast::bitmapInfo( m_d->fetch.bitmap(), jpegHeads, goodHeads, info );
        emit bitmapProcessed( jpegHeads, goodHeads, info );
    }
}

void BcastDbusCtrl::quit()
{
    qDebug()<<"qdbus:QUIT -> ";
    
    if (m_d->thread.isRunning()) {
        m_d->bcast.stop();
        m_d->thread.wait();
    }
    
    emit quitted();
}



