#include "dbusdecodeprocessor.h"
#include "org.salvum.DecodrCtrl.h"
#include "salvdbuscheck.h"
#include "eventlooprecieverfetch.h"

#include <core/controller.h>
#include <jpeg/picojpegdecodr.h>
#include <jpeg/advancedchecker.h>
#include <util/ilog.h>

#include <QTimer>
#include <QtDBus>

using namespace Log;

class DbusDecodeProcessor::Private 
{
public:
    Private(const QString &service, const QString &path, const QDBusConnection &connection, DbusDecodeProcessor *parent) 
    : dbus(new org::salvum::DecodrCtrl(service, path, connection, parent))
    
    , controller(new Core::Controller(parent))
    , fetch(new EventLoopRecieverFetch(parent))    
    , check(new SalvDbusCheck(parent))
    , decodr(new Jpeg::PicoJpegDecodr(new Jpeg::AdvancedChecker, parent))
    {
        QObject::connect(dbus, SIGNAL(exit()), parent, SLOT(onExit()) );
        QObject::connect(dbus, SIGNAL(start(int,QString)), parent, SLOT(onStart(int,QString)) );
        QObject::connect(dbus, SIGNAL(baseline(int)), check, SLOT(baseline(int)) );
        QObject::connect(check, SIGNAL(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)),
                        dbus, SLOT(fetchAtEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)) );

        QTimer *t = new QTimer(parent);
        QObject::connect(t, SIGNAL(timeout()), dbus, SLOT(heartbeat()) );
        t->setInterval(1000);
        t->start();
        
        QTimer *t2 = new QTimer(parent);
        QObject::connect(t2, SIGNAL(timeout()), parent, SLOT(checkDbus()) );
        t2->setInterval(5000);
        t2->start();
        
        // init
        controller->setEverybody(fetch, check, decodr);               
    }
    
    org::salvum::DecodrCtrl *dbus;
    
    Core::Controller        *controller;
    EventLoopRecieverFetch  *fetch;
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
    Msg("EXITING..\n");
    // it can possibly wait in 2 states: fetch and check.    
    if (m_d->check->isWaiting())
        m_d->check->breakEventLoop();
        
    m_d->fetch->exit();        

    emit exitApp();
}

void DbusDecodeProcessor::onStart(int clusterNo, const QString &shmemPath)
{
    Msg("START %s %08X\n", shmemPath.toUtf8().data(), clusterNo);
    Session(QString().sprintf("%s.%08X", shmemPath.toUtf8().data(), clusterNo));
    Msg("START %08X\n", clusterNo);
    
    m_d->fetch->init(shmemPath.toUtf8().data());
    m_d->controller->run(clusterNo);
    
    Msg("END.\n");
    onExit();
    m_d->dbus->decodingEnd(m_d->controller->success());
}

void DbusDecodeProcessor::checkDbus()
{
//    Msg("##CHECK DBUS-%s\n", m_d->dbus->isValid() ? "Valid" : "NOT Valid");
    
    if (!m_d->dbus->isValid())
        onExit();        
}

