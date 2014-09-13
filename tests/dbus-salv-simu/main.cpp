#include <QtCore>
#include <QtDBus>
#include <QtDebug>

#include <unistd.h>

#include <dbus/org.salvum.DecodrHub.h>
#include <dbus/org.salvum.DecodrCtrl.h>

#include <jpeg/imagehelpers.h>

using namespace Common;
using namespace Jpeg;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qsrand(getpid());

    org::salvum::DecodrHub hub("org.salvum.Decodr", "/hub", QDBusConnection::sessionBus());
    qDebug("HUB connect");
    if (!hub.isValid()) {
        qDebug() << "Failed to connect:" <<hub.lastError().message();
        return -1;
    }

    qDebug("aquireClient");
    QDBusPendingReply<QDBusObjectPath> r = hub.aquireClient(getpid());
    r.waitForFinished();
    if (r.isError()) {
        qDebug()<<"Failed DecodrHub::aquireClient():"<<r.error().message();
        return -2;
    }

    org::salvum::DecodrCtrl ctrl("org.salvum.Decodr", r.value().path(), QDBusConnection::sessionBus());
    qDebug("CTRL connect");
    if (!ctrl.isValid()) {
        qDebug() << "Failed to connect:" <<ctrl.lastError().message();
        return -3;
    }

    QImage img("shared_resources/1.jpg");
    if (img.isNull()) {
        qDebug()<<"Failed to load image:";
        return -4;
    }
    
    DecodedClusters dc;
    int nblocks = img.width()*img.height()/8/8;
   
    for (int i=0, c=0, previ=0; i<nblocks; ++c) {
        i += (155+qrand()%155);
        DecodedClusterInfo dci = { c, previ, qMin(nblocks-1, i) };
        dc.push_back( dci );
        previ = i + 1;
    }

    ctrl.atEnd(false, dc, RejectedClusters(), Jpeg::dbusPixmap(img));
    
    QTimer timer;
    timer.setInterval(2000);
    QObject::connect(&timer, SIGNAL(timeout()), &ctrl, SLOT(heartbeat()) );
    timer.start();

    return app.exec();
}
