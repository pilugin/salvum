#include "util/rangefileresults.h"
#include "util/ilog.h"
#include <QDir>

RangeFileResults::RangeFileResults(const QString &dir) : mDir(dir)
{
    if (!QDir().mkpath(mDir))
        Log::Msg("Failed to mkdir %s\n", mDir.toAscii().data());
}

bool RangeFileResults::restart(const QString &name)
{
    mName = name;
    mFile.setFileName( QString("%1/%2.clusters").arg(mDir, name) );
    if (!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) {
        Log::Msg("Failed to create file %s\n", mFile.fileName().toAscii().data());
        return false;
    }

    mClusters.clear();
    mLastAddedCluster = sEmptyLastAddedCluster;

    return true;
}

void RangeFileResults::addClusters(const QVector<int> &clusterNos)
{
    foreach (int c, clusterNos)
        addCluster(c);
}

void RangeFileResults::finilize(bool success)
{
    if (mLastAddedCluster != sEmptyLastAddedCluster)
        mFile.write(QString().sprintf(" %X\n", mLastAddedCluster).toAscii());
    mFile.close();

    if (!success) {
        if (!QFile::rename(mFile.fileName(), QString("%1/%2.bad.clusters").arg(mDir, mName)))
            Log::Msg("Failed to move file %s\n", mFile.fileName().toAscii().data());
    }
}

QVector<int> RangeFileResults::clusters() const
{
    return mClusters;
}

void RangeFileResults::addCluster(int clusterNo)
{
    mClusters.append(clusterNo);

    if (mLastAddedCluster == sEmptyLastAddedCluster) {
        mFile.write(QString().sprintf("%X", clusterNo).toAscii());
        mLastAddedCluster = clusterNo;

    } else if (clusterNo == (mLastAddedCluster + 1)) {
        mLastAddedCluster = clusterNo;

    } else {
        mFile.write(QString().sprintf(" %X\n%X", mLastAddedCluster, clusterNo).toAscii());
        mLastAddedCluster = clusterNo;

    }
}
