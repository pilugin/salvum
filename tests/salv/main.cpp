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
    void decode(QString file)
    {
        PicoJpegDecodr d(new AdvancedChecker);
        QFileInfo fi(file);
        QByteArray map( fi.size() / Settings::Get(Settings::ClusterSize).toInt()
                        + (fi.size() % Settings::Get(Settings::ClusterSize).toInt() == 0 ? 0 : 1 ), '1');

        DeviceMapFetch f(file, map, true);
        RangeFileResults r;

        bool rv = DecodeCtrl::decode(&d, &f, &r, 0);

        qDebug() << "RES:"<< rv;
        qDebug() << r.clusters();

//        d.image().save("0.jpg", "jpg");
    }
};

int main(int argc, char **argv)
{
    if (argc != 2) {
        qDebug( "Usage: %s inputfile", argv[0]);
        return 0;
    }

    FileLogger fl(".");
    DefaultSettings ds;

    CustomDecodeCtrl ctrl;

    ctrl.decode(argv[1]);

    return 0;
}
