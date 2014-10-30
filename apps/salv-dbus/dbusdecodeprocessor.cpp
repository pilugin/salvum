#include "dbusdecodeprocessor.h"
#include "org.salvum.DecodrCtrl.h"
#include "salvdbuscheck.h"
#include "eventlooprecieverfetch.h"

#include <core/controller.h>
#include <jpeg/picojpegdecodr.h>
#include <jpeg/advancedchecker.h>
#include <jpeg/thumbnailcreator.h>
#include <util/rangefileresult.h>
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
    , result(nullptr)
    , fetch(new EventLoopRecieverFetch(parent))    
    , check(new SalvDbusCheck(parent))
    , decodr(nullptr) //new Jpeg::PicoJpegDecodr(new Jpeg::AdvancedChecker, parent))
    , thumb(nullptr)
    {
        Jpeg::AdvancedChecker *jpegChecker = new Jpeg::AdvancedChecker;
        thumb = new Jpeg::ThumbnailCreator(parent);
        decodr = new Jpeg::PicoJpegDecodr(jpegChecker, parent);
        thumb->setSelfDelete();
        thumb->init(fetch, decodr, jpegChecker);
        QObject::connect(thumb, SIGNAL(thumbnailCreated(QString)), dbus, SLOT(thumbnailCreated(QString))    );
        QObject::connect(check, SIGNAL(progress(int,int,int)), dbus, SLOT(progress(int,int,int))            );
    
        QObject::connect(dbus, SIGNAL(exit()), parent, SLOT(onExit()) );
        QObject::connect(dbus, SIGNAL(start(int,QString,QString)), parent, SLOT(onStart(int,QString,QString)) );
        QObject::connect(dbus, SIGNAL(baseline(int)), check, SLOT(baseline(int)) );
        QObject::connect(check, SIGNAL(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::ImageInfo)),
                        dbus, SLOT(fetchAtEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::ImageInfo)) );

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
    RangeFileResult         *result;
    EventLoopRecieverFetch  *fetch;
    SalvDbusCheck           *check;
    Jpeg::PicoJpegDecodr    *decodr;    
    Jpeg::ThumbnailCreator  *thumb;
    
    QString                 wspacePath;
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

void DbusDecodeProcessor::onStart(int clusterNo, const QString &shmemPath, const QString &workspacePath)
{
    Msg("START %s %08X\n", shmemPath.toUtf8().data(), clusterNo);
    Session(QString().sprintf("%s.%08X", shmemPath.toUtf8().data(), clusterNo));
    Msg("START %08X\n", clusterNo);
    
    m_d->wspacePath = workspacePath;
    
    m_d->result = new RangeFileResult(workspacePath, this);
    m_d->result->restart( "result" );
    m_d->controller->addResult( m_d->result );    
    m_d->thumb->start(workspacePath + "/thumbnail.jpg");
    m_d->fetch->init(shmemPath.toUtf8().data());
    m_d->check->setWorkspacePath(workspacePath);
    
    m_d->dbus->progress(0, -1, -1);
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

