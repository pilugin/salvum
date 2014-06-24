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
        if (argc != 4 || !(QStringList() << "on" << "off").contains(argv[3]) ) {
            qDebug()<<"Usage: "<<argv[0]<<" shmemname brute {on,off}";
        } else {
            ctrl.request().command = Brute; 
            ctrl.request().boolparam = !qstrcmp(argv[3], "on");
        }

    } else if ( command == "stats" ) {
        ctrl.request().command = DumpStats;

    } else if ( command == "save" ) {
        if (argc != 4) {
            qDebug()<<"Usage: "<<argv[0]<<" shmemname save filename";
        } else {
            ctrl.request().command = SaveBitmap;
            ctrl.request().strparam.set(argv[3], strlen(argv[3]) +1);
        }

    } else {
        if (command != "help")
            qDebug()<<"Command "<<command<<" not found";
        qDebug()<<"Available commands: \n'pause' \n'resume' \n'exit' 'stop'\n'brute'\n'stats'\n'save'";
    }

    if (ctrl.request().command != Noop) {
        ctrl.exchange();
        qDebug("Xcg\t\tsuccess=%s", ctrl.response().success ? "Ok" : "Fail" );

        if (!ctrl.response().str.empty()) 
            qDebug()<<">>\n"<<ctrl.response().str.data();
        
    }
    
    ctrl.free();
    qDebug("Fre");

    return 0;
}

