#include "org.salvum.DecodrHub.h"
#include "org.salvum.DecodrCtrl.h"

#include <QtCore>
#include <QtDBus>
#include <QTimer>
#include <QtDebug>

#include <unistd.h>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    
    org::salvum::DecodrHub hubClient("org.salvum.Decodr", "/hub", QDBusConnection::sessionBus());
    QDBusPendingReply<QDBusObjectPath> path = hubClient.aquireClient(getpid());
    path.waitForFinished();
    if (path.isError()) {
        qDebug()<<path.error().name()<<path.error().message();
        return -1;
    }        
        
    org::salvum::DecodrCtrl decodrClient("org.salvum.Decodr", path.value().path(), QDBusConnection::sessionBus());
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &decodrClient, SLOT(heartbeat()) );
    timer.setInterval(1000);
    timer.start();
    qDebug()<<"HEARTBEAT START";        
    
    return app.exec();
}