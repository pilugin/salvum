#include <QtCore>
#include <QtDebug>
#include <devicemapfetch.h>
#include <defaultsettings.h>
#include <filelogger.h>
#include <ipcfetch/broadcast.h>

using namespace IPCFetch;

int main(int argc, char **argv)
{
    if (argc != 4) {
        qDebug( "Usage: %s inputfile bitmapfile shmem", argv[0]);
        return 0;
    }

    FileLogger l("res");
    DefaultSettings s;

    DeviceMapFetch *f = new DeviceMapFetch(QString(argv[1]), QString(argv[2]));

    QString shmem = argv[3];
    QString shmemfb = shmem + "_fb";
    Broadcast bcast(shmem.toUtf8().data(), shmemfb.toUtf8().data(), f);

    if (!bcast.isValid()) {
        qDebug()<<"Broadcast is invalid";
        return -3;
    }

    bcast.write();

    return 0;
}
