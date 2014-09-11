#include "decodrdbushub.h"
#include "salvjpegimageprovider.h"
#include "salvjpegmodel.h"
#include "bcastdbuscontroller.h"

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
    
    BcastDbusController bcastCtrl;
    
    DecodrDbusHub hub;

    QDeclarativeView view;

    SalvJpegModel model;
    QObject::connect(&hub,      SIGNAL(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*)),
                     &model,    SLOT(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*))    );
    QObject::connect(&hub,      SIGNAL(decodrClientReleased(int,QDBusObjectPath)),
                     &model,    SLOT(decodrClientReleased(int,QDBusObjectPath))                 );

    view.engine()->rootContext()->setContextProperty("bcast", &bcastCtrl);
    view.engine()->rootContext()->setContextProperty("salvJpegModel", &model);
    view.engine()->addImageProvider(model.imageProviderName(), new SalvJpegImageProvider(&model));

    view.setSource(QUrl("qml/Setup.qml"));
    view.setGeometry(0, 0, 600, 600);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);    
    
    view.show();
    return app.exec();
}
