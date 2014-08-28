#include "decodrdbushub.h"
#include <QtCore>
#include <QtDBus>
#include <QtDebug>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    
    if (! QDBusConnection::sessionBus().registerService("org.salvum.Decodr")) {
	qDebug()<<"Failed to register DBus service";
	return -1;
    }
    
    DecodrDbusHub hub;

    return app.exec();
}