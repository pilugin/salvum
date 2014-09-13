#include <QtCore>
#include <QtDebug>

#include <core/controller.h>
#include <util/devicemapfetch.h>
#include <util/simplelogger.h>
#include <util/rangefileresult.h>
#include <util/defaultsettings.h>
#include <util/simplecheck.h>
#include <jpeg/advancedchecker.h>
#include <jpeg/picojpegdecodr.h>

using namespace Core;
using namespace Jpeg;

int main(int argc, char **argv)
{
    if (argc != 4) {
        qDebug( "Usage: %s inputfile inputbitmap cluster", argv[0]);
        return 0;
    }
    
    SimpleLogger logger;
    DefaultSettings settings;
    
    QCoreApplication app(argc, argv);
    
    SimpleCheck check(true);
    DeviceMapFetch fetch;
    AdvancedChecker jpegCheck;
    PicoJpegDecodr decodr(&jpegCheck);
    RangeFileResult result;
    
    if (!fetch.init(QString(argv[1]), QString(argv[2])) ) {
        qDebug( "Failed to init the Fetch");
        return -1;
    }
    result.restart(argv[3]);
        
    Controller salv;
    salv.setEverybody( &fetch, &check, &decodr );
    salv.addResult( &result );
    
//    QObject::connect(&salv, SIGNAL(end(bool)), &app, SLOT(quit()) );
    
    qDebug("Starting Salv");
    salv.run( QString(argv[3]).toInt(nullptr, 16) );
    qDebug()<<"Exiting. Success=" << (salv.success() ? "Yes" : "No");

    return 0;//app.exec();
}
