#include <QtCore>
#include <QtDeclarative>
#include <QtDebug>

#include "salvobjectcontroller.h"

#include <util/simplelogger.h>
#include <util/defaultsettings.h>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    SimpleLogger l;
    DefaultSettings s;

    QDeclarativeView view;
    SalvObjectController ctrl;

    view.engine()->rootContext()->setContextProperty("ctrl", &ctrl);
//    view.engine()->addImageProvider(model.imageProviderName(), new SalvJpegImageProvider(&model));

    view.setSource(QUrl("qml/Main.qml"));
    view.setGeometry(0, 0, 600, 600);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.show();
    return app.exec();
}
