#include <QtCore>
#include <QtDebug>

#include "bench.h"

#include "decodectrl.h"
#include "defaultsettings.h"
#include "simplelogger.h"
#include "rangefileresults.h"

#include "jpeg/advancedchecker.h"
#include "jpeg/picojpegdecodr.h"

using namespace Jpeg;

typedef ICheck *(*FactoryFn)();

QPair<QString, FactoryFn> factories[] = { 
    qMakePair(QString("advanced"), FactoryFn([]() -> ICheck* { return new AdvancedChecker;}) ) 
};

int main(int argc, char **argv)
{
    if (argc != 3) {
        qDebug("Usage: %s input checker", argv[0]);
        return 0;
    }

    const char *input = argv[1];

    FactoryFn fn = nullptr;
    for (unsigned int i=0; i<(sizeof(factories)/sizeof(factories[0])); ++i) 
        if (factories[i].first == argv[2]) {
            fn = factories[i].second;
            break;
        }
        
    if (!fn) {
        qDebug("Unknown checker. Use one of these:");
        for (unsigned int i=0; i<(sizeof(factories)/sizeof(factories[0])); ++i) 
            qDebug("\t%s", factories[i].first.toAscii().data() );
        return 0;
    }

    ICheck *checker = fn();

    DefaultSettings ds;
    SimpleLogger sl;    

    RangeFileResults rr("res");

    Bench *b = new Bench;
    if (!b->init( checker, input )) {
        qDebug("Failed to init Bench");
        return -1;
    }

    PicoJpegDecodr d( b );

    DecodeCtrl ctrl;

    ctrl.decode( &d, b, &rr, 0 );

    qDebug("********* RESULTS ***********");
    qDebug()<<"min="<<b->minRelevance();
    qDebug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    auto cr = b->checkRelevances();
    for (int i=0; i<cr.size(); ++i) {
        qDebug()<<cr[i].first<<"\t"<<cr[i].second<< (cr[i].second < b->minRelevance() && cr[i].second >= 0  ? "\tbad" : "");
    }

    return 0;
}
