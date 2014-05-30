#include <QtCore>
#include <QtDebug>
#include "devicemapfetch.h"
#include "defaultsettings.h"
#include "filelogger.h"

using namespace Log;

class TestSettings : public DefaultSettings
{
public:
    QVariant get(int field) const
    {
        if (field == Settings::ClusterSize)
            return 1;
        else
            return DefaultSettings::get(field);
    }
};

bool run(QByteArray contents, QByteArray map, QVector<QPair<int, int> > prios, QVector<int> expect)
{
    Log::Session(contents);

    QTemporaryFile temp;
    if (!temp.open()) return false;
    qDebug("OPEN");

    temp.write(contents);
    QString fname( temp.fileName());
    temp.close();

    QVector<int> real;
    QByteArray cluster;
    int clusterNo;
    DeviceMapFetch f(fname, map);

    for (int i=0; i<prios.size(); ++i) {
        qDebug()<<prios[i]<<f.rewind(prios[i].first, prios[i].second);
        
        while (!f.atEnd()) {
            f.fetch(clusterNo, cluster);
            Msg("\n");
            qDebug()<<cluster<<clusterNo;
            
            real << clusterNo;
        }
    }

    qDebug("EXIT");
    qDebug()<<real;
    qDebug()<<expect;

    return real == expect;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestSettings  ts;
    FileLogger    fl(".");

                       //0123456789ABCDEF012
    QByteArray contents("0-234---8-ABC--F0--");
    QByteArray      map("1011+000+0111001100");
    QVector<QPair<int,int> > prios;
    prios << qMakePair(7, 9) << qMakePair(3,5)<< qMakePair(3,4) << qMakePair(7,-1);
    QVector<int> expect;
    expect << 7 << 8 << 3 << 4 << 3 << 7 << 8 << 0xa << 0xb << 0xc << 0xf << 0x10;

    Q_ASSERT( run(contents, map, prios, expect) );

    
    return 0;//a.exec();
}
