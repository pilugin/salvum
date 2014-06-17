#include <QtCore>
#include <QtDebug>

#include "bcastremotectrl.h"

#include <ipcfetch/bcastctrlcommon.h>

using namespace IPCFetch;

int main(int argc, char **argv)
{
    if (argc < 3) {
        qDebug()<<"Usage: "<<argv[0]<<" shmemname command [params]";
        return 0;
    }

    BcastRemoteCtrl ctrl(argv[1]);

    if (! ctrl.isValid() ) {
        qDebug()<<"Ctrl is invalid";
        return -2;
    }

    ctrl.acquire();
    qDebug("Ack");

    QString command = argv[2];
    if ( command == "pause" ) {
        ctrl.request().command = Pause;

    } else if ( command == "resume" ) {
        ctrl.request().command = Resume;

    } else if ( command == "exit" || command == "stop" ) {
        ctrl.request().command = Exit;

    } else if ( command == "brute" ) {

    } else if ( command == "stats" ) {

    } else if ( command == "save" ) {

    }

    ctrl.exchange();
    qDebug("Xcg");
    
    qDebug("success=%s", ctrl.response().success ? "Ok" : "Fail" );

    ctrl.free();
    qDebug("Fre");

    return 0;
}

