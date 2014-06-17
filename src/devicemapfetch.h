#ifndef DEVICEMAPFETCH_H
#define DEVICEMAPFETCH_H

#include "if/ifetch.h"
#include <QByteArray>
#include <QQueue>
#include <QPair>
#include <QFile>

class DeviceMapFetch : public IFetch
{
public:
    DeviceMapFetch(const QString &file, const QString &mapFile, bool brute=false);
    DeviceMapFetch(const QString &file, const QByteArray &map, bool brute=false);

    void fetch(int &clusterNo, QByteArray &cluster);
    bool rewind(int clusterNo, int clusterNoEnd =InvalidClusterNo);
    void skip(const QVector<int> &clusters);
    void fastfwd();
    bool atEnd() const;
    QByteArray bitmap() const { return mMap; }

    void setBrute(bool brute) { mBrute = brute; }

protected:
    QByteArray mMap;
    int mCurrentCluster;
    int mClusterEnd;

    QFile mFile;

    bool mBrute;

    void postfetch();
};

#endif // DEVICEMAPFETCH_H
