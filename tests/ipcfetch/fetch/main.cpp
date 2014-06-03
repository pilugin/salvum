#include <QtCore>
#include <QtDebug>
#include <ipcfetch/broadcast.h>
#include <ipcfetch/recieverfetch.h>
#include <defaultsettings.h>
#include <simplelogger.h>
#include <devicemapfetch.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

using namespace IPC;
using namespace IPCFetch;
//using namespace 

int main(int argc, char **argv)
{
    if (argc < 2 || (strcmp(argv[1], "bcast") && strcmp(argv[1], "recv") )) {
        qDebug()<<"usage: "<<argv[0]<<" <bcast|recv>";
        return 0;
    }

    SimpleLogger l;
    DefaultSettings s;

    const char *shmem = "/yobo";
    const char *shmemfb = "/yobo_fb";

    if ( !strcmp(argv[1], "bcast") ) {

        QByteArray map;
        for (int i=0; i<10000; ++i)
            map.push_back( i%2 ? '0' : '1' );
        IFetch *f = new DeviceMapFetch("/dev/zero", map);

        Broadcast bs(shmem, shmemfb, f);

        qDebug()<<"BS created. valid:"<<bs.isValid();
    
        bs.write();

    } else {

        sleep(2);

        RecieverFetch f(shmem, shmemfb);
        qDebug()<<"RF created. valid:"<<f.isValid();

        sleep(1);
        qDebug()<<"RF rewind";
        f.rewind(9970);

        int clusterNo;
        QByteArray ba;

        sleep(1);
        qDebug()<<"RF about 2 fetch";
        while (!f.atEnd()) {
            f.fetch(clusterNo, ba);
            qDebug()<<"RF fetch:"<<clusterNo;
        }

        f.rewind(0);
        qDebug()<<"RF about 2 fetch 2";
        while (!f.atEnd()) {
            f.fetch(clusterNo, ba);
            qDebug()<<"RF fetch:"<<clusterNo;
        }
    }
    

    return 0;
}