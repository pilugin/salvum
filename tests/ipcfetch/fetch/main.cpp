#include <QtCore>
#include <QtDebug>

#include <ipcfetch/broadcast.h>
#include <ipcfetch/recieverfetch.h>
#include <ipcfetch/feedbackresults.h>
#include <defaultsettings.h>
#include <simplelogger.h>
#include <filelogger.h>
#include <devicemapfetch.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#include "createtestmedia.h"

using namespace IPC;
using namespace IPCFetch;
//using namespace 

int main(int argc, char **argv)
{

    if (argc < 2 || (strcmp(argv[1], "bcast") && strcmp(argv[1], "recv") )) {
        qDebug()<<"usage: "<<argv[0]<<" <bcast|recv>";
        return 0;
    }

//    SimpleLogger l;
    FileLogger l("log");
    DefaultSettings s;

    const char *shmem = "/yobo";
    const char *shmemfb = "/yobo_fb";

    if ( !strcmp(argv[1], "bcast") ) {

        l.setSession("BCAST");

        if (!createTestMedia("MEDIA", "BITMAP")) {
            qDebug("Failed to create media/bitmap files");
            return -2;
        }
        qDebug("media/bitmap ready");

        IFetch *f = new DeviceMapFetch(QString("MEDIA"), QString("BITMAP"));

        Broadcast bs(shmem, shmemfb, f);

        qDebug()<<"BS created. valid:"<<bs.isValid();
    
        bs.write();

    } else {

        RecieverFetch f(shmem);
        FeedbackResults r(shmemfb, "log");

        qDebug()<<"RF created. valid:"<<f.isValid();
        qDebug()<<"FR created. valid:"<<r.isValid();
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