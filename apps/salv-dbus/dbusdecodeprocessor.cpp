#include "dbusdecodeprocessor.h"
#include "org.salvum.DecodrCtrl.h"
#include "salvdbuscheck.h"

#include <core/controller.h>
#include <jpeg/picojpegdecodr.h>
#include <jpeg/advancedchecker.h>
#include <ipcfetch/recieverfetch.h>

#include <QTimer>
#include <QtDBus>

class DbusDecodeProcessor::Private 
{
public:
    Private(const QString &service, const QString &path, const QDBusConnection &connection, DbusDecodeProcessor *parent) 
    : dbus(new org::salvum::DecodrCtrl(service, path, connection, parent))
    
    , controller(new Core::Controller(parent))
    , fetch(new IPCFetch::RecieverFetch(parent))    
    , check(new SalvDbusCheck(parent))
    , decodr(new Jpeg::PicoJpegDecodr(new Jpeg::AdvancedChecker, parent))
    {
        QObject::connect(dbus, SIGNAL(exit()), parent, SLOT(onExit()) );
        QObject::connect(dbus, SIGNAL(start(int,QString)), parent, SLOT(onStart(int,QString)) );
//        QObject::connect(dbus, SIGNAL(resume()), parent, SLOT(onResume()) ); <<=== do we really need this??
        QObject::connect(dbus, SIGNAL(baseline(int)), check, SLOT(baseline(int)) );
        QObject::connect(check, SIGNAL(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)),
                        dbus, SLOT(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)) );

        QTimer *t = new QTimer(parent);
        QObject::connect(t, SIGNAL(timeout()), dbus, SLOT(heartbeat()) );
        t->setInterval(1000);
        t->start();
        
        // init
        controller->setEverybody(fetch, check, decodr);
    }
    
    org::salvum::DecodrCtrl *dbus;
    
    Core::Controller        *controller;
    IPCFetch::RecieverFetch *fetch;
    SalvDbusCheck           *check;
    Jpeg::PicoJpegDecodr    *decodr;
};

/////////////////////////////////////////////////

DbusDecodeProcessor::DbusDecodeProcessor(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
: QObject(parent)
, m_d(new Private(service, path, connection, this))
{
}

DbusDecodeProcessor::~DbusDecodeProcessor()
{
    delete m_d;
}

void DbusDecodeProcessor::onExit()
{
    if (m_d->check->isWaiting())
        m_d->check->breakEventLoop();

    emit exitApp();
}

void DbusDecodeProcessor::onStart(int clusterNo, const QString &shmemPath)
{
    qDebug()<<__FUNCTION__<<clusterNo<<shmemPath;
    
    m_d->fetch->init(shmemPath.toUtf8().data());
    m_d->controller->run(clusterNo);
}
