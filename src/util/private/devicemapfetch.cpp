#include "util/devicemapfetch.h"
#include "util/ilog.h"
#include "util/isettings.h"

using namespace Log;
using namespace Settings;
using namespace Core3;
using namespace Common;

DeviceMapFetch::DeviceMapFetch()
{
}

bool DeviceMapFetch::init(const QString &file, const QByteArray &map, bool brute)
{
    clear();
    mFile.setFileName(file);
    if (!mFile.open(QFile::ReadOnly)) {
        Msg("Failed to open file %s '%s'", file.toAscii().data(), mFile.errorString().toAscii().data());
        return false;
    }

    mMap = map;
    mBrute = brute;

    return true;
}

bool DeviceMapFetch::init(const QString &file, const QString &mapFile, bool brute)
{
    clear();
    mFile.setFileName(file);
    if (!mFile.open(QFile::ReadOnly)) {
        Msg("Failed to open file %s '%s'", file.toAscii().data(), mFile.errorString().toAscii().data());
        return false;
    }

    QFile map(mapFile);
    if (!map.open(QFile::ReadOnly))
        Msg("Failed to open file %s '%s'", mapFile.toAscii().data(), map.errorString().toAscii().data());

    mMap = map.readAll();
    mBrute = brute;

    return true;
}

void DeviceMapFetch::clear()
{
    mFile.close();
    mMap.clear();
}

Cluster DeviceMapFetch::doFetch()
{
    Cluster cluster;

    Msg("\n[% 8X", mCurrentCluster);
    if (! mFile.seek(qint64(mCurrentCluster) * Get(ClusterSize).toInt())) {
        Msg(":Failed]");
        cluster.first = InvalidClusterNo;

    } else {
        cluster.second = mFile.read(Get(ClusterSize).toInt());
        cluster.first = mCurrentCluster;
        Msg("]");

        postfetch();
    }

    return cluster;
}

void DeviceMapFetch::postfetch()
{
    static const QByteArray goodClusters(Get(GoodClusters).toByteArray());

    ++mCurrentCluster;
    if (!goodClusters.contains(mMap[mCurrentCluster]) ) //< not guided. if next section is invalid - skip it
        fastfwd();
}

void DeviceMapFetch::rewind(int clusterNo)
{
    mCurrentCluster = clusterNo;
//    return !atEnd()     && !mMap.isEmpty()      && mFile.isOpen();
}

void DeviceMapFetch::fastfwd()
{
    static const QByteArray goodClusters(Get(GoodClusters).toByteArray());

    if (mBrute) { //< brute force mode
        while (mCurrentCluster < mMap.size()
                && ! goodClusters.contains(mMap[ mCurrentCluster ])  )
            ++mCurrentCluster;

    } else { //<
        if (mCurrentCluster == 0)
            ++mCurrentCluster;
        while (mCurrentCluster < mMap.size()
                && ! ( goodClusters.contains(mMap[ mCurrentCluster ]) && !goodClusters.contains(mMap[ mCurrentCluster-1 ])   )  )
            ++mCurrentCluster;
    }
}

bool DeviceMapFetch::atEnd() const
{
    return mCurrentCluster >= mMap.size();
}

void DeviceMapFetch::skipClusters(const QList<int> &clusters)
{
    static const char used = Get( UsedCluster ).toChar().toAscii();

    Msg("\n[FSKIP: #=%d, (%08X, %08X)]", clusters.size(), clusters.front(), clusters.back());

    for (int i=0; i<clusters.size(); ++i)
        if (clusters[i] < mMap.size())
            mMap[ clusters[i] ] = used;

}


