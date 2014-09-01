#include "decodrdbushub.h"
#include "salvjpegimageprovider.h"
#include "salvjpegmodel.h"

#include <QtCore>
#include <QtDeclarative>
#include <QtDBus>
#include <QtDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    if (! QDBusConnection::sessionBus().registerService("org.salvum.Decodr")) {
        qDebug()<<"Failed to register DBus service";
        return -1;
    }
    
    DecodrDbusHub hub;

    QDeclarativeView view;

    SalvJpegModel model;
    QObject::connect(&hub,      SIGNAL(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*)),
                     &model,    SLOT(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*))    );
    QObject::connect(&hub,      SIGNAL(decodrClientReleased(int,QDBusObjectPath)),
                     &model,    SLOT(decodrClientReleased(int,QDBusObjectPath))                 );

    view.engine()->rootContext()->setContextProperty("salvJpegModel", &model);
    view.engine()->addImageProvider(model.imageProviderName(), new SalvJpegImageProvider(&model));

    view.setSource(QUrl(/*"qrc:/"*/ "qml/Main.qml"));
    view.show();
    return app.exec();
}
