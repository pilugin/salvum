#include <QtCore>
#include <QtDebug>
#include <filelogger.h>
#include <defaultsettings.h>

#include "bcastctrl.h"


int main(int argc, char **argv)
{
    if (argc != 4) {
        qDebug( "Usage: %s inputfile bitmapfile shmem", argv[0]);
        return 0;
    }

    FileLogger l("res");
    DefaultSettings s;

    QByteArray shmem = argv[3];
    QByteArray shmemfb = shmem + "_fb";
    QByteArray shmemctrl = shmem + "_ctrl";

    BcastCtrl bcastCtrl(shmem.data(), shmemfb.data(), shmemctrl.data(), argv[1], argv[2]);

    if (!bcastCtrl.isValid()) {
        qDebug()<<"Broadcast is invalid";
        return -3;
    }

    bcastCtrl.exec();

    return 0;
}
