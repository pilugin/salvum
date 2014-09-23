#include "org.salvum.DecodrHub.h"
//#include "org.salvum.DecodrCtrl.h"
#include "dbusdecodeprocessor.h"

#include <util/filelogger.h>
#include <util/defaultsettings.h>

#include <QtCore>
#include <QtDBus>
#include <QTimer>
#include <QtDebug>

#include <unistd.h>

int main(int argc, char **argv)
{
    // singletons
    QCoreApplication app(argc, argv);
    FileLogger l("res");
    DefaultSettings s;
    
    Log::Session(QString("TMP_%1").arg(getpid()));
    Log::Msg("Starting %s\n", argv[0]);
    
    // connect to /hub
    org::salvum::DecodrHub hubClient("org.salvum.Decodr", "/hub", QDBusConnection::sessionBus());
    QDBusPendingReply<QDBusObjectPath> path = hubClient.aquireClient(getpid());
    path.waitForFinished();
    if (path.isError()) {
        Log::Msg("%s:%d: %s %s\n", __FILE__, __LINE__, path.error().name().toUtf8().data(), path.error().message().toUtf8().data() );
        return -1;
    }        
    

#if 0        
    org::salvum::DecodrCtrl decodrClient("org.salvum.Decodr", path.value().path(), QDBusConnection::sessionBus());
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &decodrClient, SLOT(heartbeat()) );
    timer.setInterval(1000);
    timer.start();
    qDebug()<<"HEARTBEAT START";        
#else

    DbusDecodeProcessor proc("org.salvum.Decodr", path.value().path(), QDBusConnection::sessionBus());
    QObject::connect(&proc, SIGNAL(exitApp()), &app, SLOT(quit()) );

#endif    
    
    return app.exec();
}