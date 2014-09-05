#include <QtCore>
#include <QtDebug>
#include "decodectrl.h"
#include "devicemapfetch.h"
#include "defaultsettings.h"
#include "filelogger.h"
#include "rangefileresults.h"
#include "jpeg/picojpegdecodr.h"
#include "jpeg/advancedchecker.h"

using namespace Log;
using namespace Jpeg;

class CustomDecodeCtrl : public DecodeCtrl
{
public:
    void decode(QString file, QString map, int clusterNo)
    {
        PicoJpegDecodr d(new AdvancedChecker);
        DeviceMapFetch f(file, map);
        RangeFileResults r("res");

        bool rv = DecodeCtrl::decode(&d, &f, &r, clusterNo);

        qDebug() << "RES:"<< rv;
        qDebug() << r.clusters();

        d.image().save(QString().sprintf("res/%x.jpg", clusterNo), "jpg");
    }
};

int main(int argc, char **argv)
{
    if (argc != 4) {
        qDebug( "Usage: %s inputfile bitmapfile cluster", argv[0]);
        return 0;
    }

    FileLogger fl("res");
    DefaultSettings ds;

    CustomDecodeCtrl ctrl;

    ctrl.decode(argv[1], argv[2], QString(argv[3]).toInt(nullptr, 16));

    return 0;
}
