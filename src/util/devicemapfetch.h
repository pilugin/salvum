#ifndef DEVICEMAPFETCH_H
#define DEVICEMAPFETCH_H

#include <core-3/fetch.h>
#include <QByteArray>
#include <QQueue>
#include <QPair>
#include <QFile>

class DeviceMapFetch : public Core3::Fetch
{
public:
    DeviceMapFetch();
    bool init(const QString &file, const QString &mapFile, bool brute=false);
    bool init(const QString &file, const QByteArray &map, bool brute=false);
    void setBrute(bool brute) { mBrute = brute; }

    void rewind(int clusterNo =Common::InvalidClusterNo);
    void skipClusters(const QList<int> &clusters);
    void fastfwd();
    bool atEnd() const;
    QByteArray bitmap() const { return mMap; }

protected:
    QByteArray mMap;
    int mCurrentCluster;
    QFile mFile;
    bool mBrute;

    Common::Cluster doFetch();
    void postfetch();
    void clear();
};

#endif // DEVICEMAPFETCH_H
