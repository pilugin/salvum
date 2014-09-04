#ifndef DEVICEMAPFETCH_H
#define DEVICEMAPFETCH_H

#include <core/fetch.h>
#include <QByteArray>
#include <QQueue>
#include <QPair>
#include <QFile>

class DeviceMapFetch : public Fetch
{
public:
    DeviceMapFetch(QObject *parent =nullptr);    
    bool init(const QString &file, const QString &mapFile, bool brute=false);
    bool init(const QString &file, const QByteArray &map, bool brute=false);

    void fetch(int &clusterNo, QByteArray &cluster);
    bool rewind(int clusterNo);
    void skip(int clusterNo, int length);
    void fastfwd();
    bool atEnd() const;
    QByteArray bitmap() const { return mMap; }

    void setBrute(bool brute) { mBrute = brute; }

protected:
    QByteArray mMap;
    int mCurrentCluster;

    QFile mFile;

    bool mBrute;

    void postfetch();
};

#endif // DEVICEMAPFETCH_H
