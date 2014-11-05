#include <QtCore>
#include <QtDebug>

#include <core-3/decoding.h>
#include <util/devicemapfetch.h>
#include <util/simplelogger.h>
#include <util/defaultsettings.h>
#include <util/simplecheck.h>

#include <jpeg/archive.h>
#include <jpeg/advancedchecker.h>
#include <jpeg/picojpegdecodr.h>

int main(int argc, char **argv)
{
    if (argc != 4) {
        qDebug( "Usage: %s inputfile inputbitmap cluster", argv[0]);
        return 0;
    }
    
    SimpleLogger logger;
    DefaultSettings settings;
    
    QCoreApplication app(argc, argv);
    
    SimpleCheck<Jpeg::DecodrState> check;
    DeviceMapFetch fetch;
    Jpeg::AdvancedChecker jpegCheck;
    Jpeg::PicoJpegDecodr decodr(&jpegCheck);
    Jpeg::Archive archive("SALV");

    QObject::connect(&archive, SIGNAL(thumbnailCreated(QString)), &jpegCheck, SLOT(addThumbnail(QString)) );
    
    if (!fetch.init(QString(argv[1]), QString(argv[2])) ) {
        qDebug( "Failed to init the Fetch");
        return -1;
    }
    
    int clusterNo = QString(argv[3]).toInt(nullptr, 16);
    qDebug("Starting Salv (%08X)", clusterNo);

    bool result = Core3::decoding(clusterNo, fetch, decodr, check, archive);

    qDebug()<<"Exiting. Success=" << (result ? "Yes" : "No");

    return 0;//app.exec();
}
