#include "org.salvum.DecodrHub.h"
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
    
    DbusDecodeProcessor proc("org.salvum.Decodr", path.value().path(), QDBusConnection::sessionBus());
    QObject::connect(&proc, SIGNAL(exitApp()), &app, SLOT(quit()) );
    
    return app.exec();
}