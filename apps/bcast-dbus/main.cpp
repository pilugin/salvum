#include "bcastdbusctrl.h"
#include <QtCore>
#include <QtDBus>
#include <QtDebug>

#include <filelogger.h>
#include <defaultsettings.h>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    
    FileLogger l("res");
    DefaultSettings s;

    if (argc != 3) {
	QTextStream(stdout) << "usage:" << argv[0] << " <mediafile> <bitmapfile>\n";
	return 0;
    }

    BcastDbusCtrl bcast(argv[1], argv[2]);
    
    if (! bcast.isValid()) {
	qDebug()<<"Failed to initialize Broadcast";
	return -1;
    }
    
    QObject::connect( &bcast, SIGNAL(stopped()), &app, SLOT(quit()) );
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
    
    if (! conn.registerObject( "/broadcast", &bcast )) {
	qDebug()<<"Failed to register DBus object";
	return -4;
    }

    return app.exec();
}
