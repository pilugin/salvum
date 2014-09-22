#include "dbusdecodeprocessor.h"
#include "org.salvum.DecodrCtrl.h"
#include "salvdbuscheck.h"
#include <QTimer>
#include <QtDBus>

class DbusDecodeProcessor::Private 
{
public:
    Private(const QString &service, const QString &path, const QDBusConnection &connection, DbusDecodeProcessor *parent) 
    : dbus(new org::salvum::DecodrCtrl(service, path, connection, parent))
    , check(new SalvDbusCheck(parent))
    {
        QObject::connect(dbus, SIGNAL(exit()), parent, SLOT(onExit()) );
        QObject::connect(dbus, SIGNAL(start(int)), parent, SLOT(onStart(int)) );
//        QObject::connect(dbus, SIGNAL(resume()), parent, SLOT(onResume()) ); <<=== do we really need this??
        QObject::connect(dbus, SIGNAL(baseline(int)), check, SLOT(baseline(int)) );
        QObject::connect(check, SIGNAL(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)),
                        dbus, SLOT(atEnd(bool, Common::DecodedClusters, Common::RejectedClusters, Common::Pixmap)) );

        QTimer *t = new QTimer(parent);
        QObject::connect(t, SIGNAL(timeout()), dbus, SLOT(heartbeat()) );
        t->setInterval(1000);
        t->start();
    }
    
    org::salvum::DecodrCtrl *dbus;
    SalvDbusCheck *check;
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

void DbusDecodeProcessor::onStart(int clusterNo)
{
    
}
