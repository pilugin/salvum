#include "bcastdbusctrl.h"
#include <QtCore>
#include <QtDBus>
#include <QtDebug>

#include <util/filelogger.h>
#include <util/defaultsettings.h>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    
    FileLogger l("res");
    DefaultSettings s;

    BcastDbusCtrl bcastCtrl;
    
    if (! bcastCtrl.isValid()) {
        qDebug()<<"Failed to initialize Broadcast";
        return -1;
    }
    
    QObject::connect( &bcastCtrl, SIGNAL(quitted()), &app, SLOT(quit()) );
//    bcast.start();

    QDBusConnection conn = QDBusConnection::sessionBus();
    
    if (! conn.isConnected()) {
        qDebug()<<"Failed to connect to DBus session bus";
        return -2;
    }
    
    if (! conn.registerService( "org.salvum.Broadcast" )) {
        qDebug()<<"Failed to register DBus service";
        return -3;
    }
    
    if (! conn.registerObject( "/broadcast", &bcastCtrl )) {
        qDebug()<<"Failed to register DBus object";
        return -4;
    }

    return app.exec();
}
