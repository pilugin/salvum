#include "decodrdbushub.h"
#include "salvjpegimageprovider.h"
#include "salvjpegmodel.h"
#include "bcastdbuscontroller.h"
#include "jpegheadsmodel.h"

#include <ui/bitmapinfomodel.h>

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
    BitmapInfoModel bitmapInfo;
    JpegHeadsModel jpegHeadsModel;
    JpegHeadsModel goodHeadsModel;
    
    QObject::connect(&bcastCtrl, SIGNAL(bitmapInfoUpdated(BitmapInfo)), &bitmapInfo, SLOT(setInfo(BitmapInfo)));
    QObject::connect(&bcastCtrl, SIGNAL(jpegHeadsUpdated(QList<int>)), &jpegHeadsModel, SLOT(setHeads(QList<int>)));
    QObject::connect(&bcastCtrl, SIGNAL(goodHeadsUpdated(QList<int>)), &goodHeadsModel, SLOT(setHeads(QList<int>)));
    
    DecodrDbusHub hub;

    QDeclarativeView view;

    SalvJpegModel model;
    QObject::connect(&hub,      SIGNAL(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*)),
                     &model,    SLOT(decodrClientAdded(int,QDBusObjectPath,DecodrDbusCtrl*))    );
    QObject::connect(&hub,      SIGNAL(decodrClientReleased(int,QDBusObjectPath)),
                     &model,    SLOT(decodrClientReleased(int,QDBusObjectPath))                 );

    view.engine()->rootContext()->setContextProperty("bcast", &bcastCtrl);
    view.engine()->rootContext()->setContextProperty("salvJpegModel", &model);
    view.engine()->rootContext()->setContextProperty("bitmapInfo", &bitmapInfo);    
    view.engine()->rootContext()->setContextProperty("jpegHeadsModel", &jpegHeadsModel);
    view.engine()->rootContext()->setContextProperty("goodHeadsModel", &goodHeadsModel);
    view.engine()->addImageProvider(model.imageProviderName(), new SalvJpegImageProvider(&model));

    view.setSource(QUrl("qml/Setup.qml"));
    view.setGeometry(0, 0, 600, 600);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);    
    
    view.show();
    return app.exec();
}
