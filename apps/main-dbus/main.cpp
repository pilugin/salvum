#include "supervisor.h"
#include "decodrdbushub.h"
#include "bcastdbuscontroller.h"
#include "models/jpegheadsmodel.h"
#include "models/selectedheadsmodel.h"
//#include "models/salvjpegmodel.h"
#include "models/workspacemodel.h"

#include <ui/bitmapinfomodel.h>
#include <ui/qobjectlistmodel.h>
#include <ui/imageprovider.h>

#include <QtCore>
#include <QtDeclarative>
#include <QtDBus>
#include <QtDebug>


using namespace Ui;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    // init DBus    
    if (! QDBusConnection::sessionBus().registerService("org.salvum.Decodr")) {
        qDebug()<<"Failed to register DBus service";
        return -1;
    }
    
    // Everybody!
    Supervisor supervisor;
    
    DecodrDbusHub decoderHub;
    BcastDbusController bcastCtrl;
    BitmapInfoModel bitmapInfo;
    JpegHeadsModel jpegHeadsModel;
    JpegHeadsModel goodHeadsModel;
    WorkspaceModel wspaceModel;
    SelectedHeadsModel selectedHeadsModel;    

    decoderHub.setWorkspaceModel( &wspaceModel );
    selectedHeadsModel.setWorkspaceModel( &wspaceModel );

    QObject::connect(&bcastCtrl, SIGNAL(bitmapInfoUpdated(Common::BitmapInfo)), &bitmapInfo, SLOT(setInfo(Common::BitmapInfo)));
    QObject::connect(&bcastCtrl, SIGNAL(jpegHeadsUpdated(QList<int>)), &jpegHeadsModel, SLOT(setHeads(QList<int>)));
    QObject::connect(&bcastCtrl, SIGNAL(goodHeadsUpdated(QList<int>)), &goodHeadsModel, SLOT(setHeads(QList<int>)));
    QObject::connect(&bcastCtrl, SIGNAL(jpegHeadsUpdated(QList<int>)), &selectedHeadsModel, SLOT(clear()));
    QObject::connect(&bcastCtrl, SIGNAL(goodHeadsUpdated(QList<int>)), &selectedHeadsModel, SLOT(clear()));
    
    QObject::connect(&jpegHeadsModel, SIGNAL(headSelected(int,bool)), &selectedHeadsModel, SLOT(onHeadSelected(int,bool)));
    QObject::connect(&goodHeadsModel, SIGNAL(headSelected(int,bool)), &selectedHeadsModel, SLOT(onGoodHeadSelected(int,bool)));
        
    // export to QML
    QDeclarativeView view;
    view.engine()->rootContext()->setContextProperty("supervisor", &supervisor);
    view.engine()->rootContext()->setContextProperty("bcast", &bcastCtrl);
    view.engine()->rootContext()->setContextProperty("bitmapInfo", &bitmapInfo);    
    view.engine()->rootContext()->setContextProperty("jpegHeadsModel", &jpegHeadsModel);
    view.engine()->rootContext()->setContextProperty("goodHeadsModel", &goodHeadsModel);
    view.engine()->rootContext()->setContextProperty("selectedHeadsModel", &selectedHeadsModel);
    view.engine()->rootContext()->setContextProperty("wspaceModel", &wspaceModel);
    view.engine()->rootContext()->setContextProperty("decoderHub", &decoderHub);
    
    Ui::ImageProvider *imgPrv = new Ui::ImageProvider;
    imgPrv->setName("prov");
    imgPrv->addAdaptor("decoderHub", &decoderHub);
    view.engine()->addImageProvider(imgPrv->name(), imgPrv);

    // init view
    view.setSource(QUrl("qml/Main.qml"));
    view.setGeometry(0, 0, 600, 600);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);    
    
    // run!
    view.show();
    return app.exec();
}
