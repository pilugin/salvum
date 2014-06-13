#include "devicemapfetch.h"
#include "if/ilog.h"
#include "if/isettings.h"

using namespace Log;
using namespace Settings;

DeviceMapFetch::DeviceMapFetch(const QString &file, const QByteArray &map, bool brute)
{
    mFile.setFileName(file);
    if (!mFile.open(QFile::ReadOnly))
        Msg("Failed to open file %s '%s'", file.toAscii().data(), mFile.errorString().toAscii().data());

    mMap = map;
    mBrute = brute;
}

DeviceMapFetch::DeviceMapFetch(const QString &file, const QString &mapFile, bool brute)
{
    mFile.setFileName(file);
    if (!mFile.open(QFile::ReadOnly))
        Msg("Failed to open file %s '%s'", file.toAscii().data(), mFile.errorString().toAscii().data());

    QFile map(mapFile);
    if (!map.open(QFile::ReadOnly))
        Msg("Failed to open file %s '%s'", mapFile.toAscii().data(), map.errorString().toAscii().data());

    mMap = map.readAll();
    mBrute = brute;
}


void DeviceMapFetch::fetch(int &clusterNo, QByteArray &cluster)
{
    Msg("\n");

    cluster.clear();
    if (atEnd()) {
        Msg("[Fetch End]");
        return;
    }

    Msg("[% 8X", mCurrentCluster);
    if (! mFile.seek(qint64(mCurrentCluster) * Get(ClusterSize).toInt())) {
        Msg(":Failed]");
        clusterNo = InvalidClusterNo;

    } else {
        cluster = mFile.read(Get(ClusterSize).toInt());
        clusterNo = mCurrentCluster;
        Msg("]");

        postfetch();
    }
}

void DeviceMapFetch::postfetch()
{
    static const QByteArray goodClusters(Get(GoodClusters).toByteArray());

    ++mCurrentCluster;
    if (mClusterEnd == InvalidClusterNo
                && !goodClusters.contains(mMap[mCurrentCluster]) ) //< not guided. if next section is invalid - skip it
        fastfwd();
}

bool DeviceMapFetch::rewind(int clusterNo, int clusterNoEnd)
{
    mCurrentCluster = clusterNo;
    mClusterEnd = clusterNoEnd;
    return !atEnd()     && !mMap.isEmpty()      && mFile.isOpen();
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
    if (mClusterEnd == InvalidClusterNo)
        return mCurrentCluster >= mMap.size();
    else
        return mCurrentCluster >= mClusterEnd;
}

void DeviceMapFetch::skip(const QVector<int> &clusters)
{
    static const char used = Get( UsedCluster ).toChar().toAscii();

    Msg("\n[FSKIP: #=%d, (%08X, %08X)]", clusters.size(),
                clusters.size() ? clusters.front() : -1,
                clusters.size() ? clusters.back() : -1);

    for (int i=0; i<clusters.size(); ++i)
        if (clusters[i] < mMap.size())
            mMap[ clusters[i] ] = used;

}


