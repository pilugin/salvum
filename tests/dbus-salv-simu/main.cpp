#include <QtCore>
#include <QtDBus>
#include <QtDebug>

#include <unistd.h>

#include "org.salvum.DecodrHub.h"
#include "org.salvum.DecodrCtrl.h"

#include <jpeg/imagehelpers.h>
#include "decoder.h"

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

    QTimer timer;
    timer.setInterval(2000);
    QObject::connect(&timer, SIGNAL(timeout()), &ctrl, SLOT(heartbeat()) );
    timer.start();

    Decoder d(&ctrl);

    return app.exec();
}
