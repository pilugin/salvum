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
#include "testdecodectrl.h"
#include "decoder.h"

using namespace IPC;
using namespace IPCFetch;
//using namespace 

int main(int argc, char **argv)
{

    if (argc < 2 || (strcmp(argv[1], "bcast") && strcmp(argv[1], "recv") )) {
        qDebug()<<"usage: "<<argv[0]<<" <bcast|recv>";
        return 0;
    }

    DefaultSettings s;

    const char *shmem = "/yobo";
    const char *shmemfb = "/yobo_fb";

    if ( !strcmp(argv[1], "bcast") ) {

        FileLogger l("log");

        l.setSession("BCAST");

        if (!createTestMedia("MEDIA", "BITMAP")) {
            qDebug("Failed to create media/bitmap files");
            return -2;
        }
        qDebug("media/bitmap ready");

        IFetch *f = new DeviceMapFetch(QString("MEDIA"), QString("BITMAP"), true);

        Broadcast bs(shmem, shmemfb, f);

        qDebug()<<"BS created. valid:"<<bs.isValid();
    
        bs.write();

    } else {

        //SimpleLogger l;
        FileLogger l("log");

        if (argc != 3) {
            qDebug()<<"Usage: " << argv[0] << " " << argv[1] << " <clusterNumberHex>";
            return 0;
        }
        bool ok = false;
        int clusterNo = QString(argv[2]).toInt(&ok, 16);
        if (!ok) {
            qDebug()<<"Failed to parse clusterNumber: "<<argv[2];
            return 0;
        }

        RecieverFetch f(shmem);
        FeedbackResults r(shmemfb, "log");

        qDebug()<<"RF created. valid:"<<f.isValid();
        qDebug()<<"FR created. valid:"<<r.isValid();

        if (!f.isValid() || !r.isValid())
            return false;

        Decoder d;

        TestDecodeCtrl ctrl;

        bool res = ctrl.decode(&d, &f, &r, clusterNo);

        qDebug()<<"Decode"<<res;
    }
    

    return 0;
}
