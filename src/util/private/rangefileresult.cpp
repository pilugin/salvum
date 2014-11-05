#include "util/rangefileresult.h"
#include "util/ilog.h"
#include <functional>
#include <QDir>
#include <QtDebug>

RangeFileResult::RangeFileResult(const QString &dir) : mDir(dir)
{
    if (!QDir().mkpath(mDir))
        Log::Msg("Failed to mkdir %s\n", mDir.toUtf8().data());
}

void RangeFileResult::restart(const QString &clustersName, const QString &dataName)
{
    mFile.setFileName(      QString("%1/%2").arg(mDir, clustersName) );
    mDataFile.setFileName(  QString("%1/%2").arg(mDir, dataName) );
    
    if (!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mFile.fileName().toUtf8().data());
    if (!mDataFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mDataFile.fileName().toUtf8().data());

    mClusterRange.len = 0;
}

void RangeFileResult::addCluster(const Common::Cluster &cluster)
{
    if (mDataFile.write(cluster.second) == -1)
        Log::Msg("Failed to Result::addCluster. %s", mDataFile.errorString().toUtf8().data());
    store(cluster.first);
}

void RangeFileResult::flush()
{
    QByteArray data;
    if (mClusterRange.len == 1) {
        data = QString().sprintf("%08X\n", mClusterRange.beginCluster).toUtf8();

    } else if (mClusterRange.len > 1) {
        data = QString().sprintf("%08X %08X\n", mClusterRange.beginCluster, mClusterRange.beginCluster+mClusterRange.len-1).toUtf8();

    }

    if (data.size()>0)
        if (mFile.write(data) == -1)
            Log::Msg("Failed to Result::addCluster. %s", mFile.errorString().toUtf8().data());
    mClusterRange.len = 0;
}

void RangeFileResult::store(int beginCluster)
{
    if (beginCluster == (mClusterRange.beginCluster + mClusterRange.len)) {
        ++mClusterRange.len;
    } else {
        flush();
        mClusterRange.beginCluster = beginCluster;
        mClusterRange.len = 1;
    }
}

void RangeFileResult::finalize(bool success)
{
    flush();
    mFile.close();
    mDataFile.close();

    if (!success) {
        if (!QFile::rename(mFile.fileName(), QString("%1/clusters.bad").arg(mDir)))
            Log::Msg("Failed to move file %s\n", mFile.fileName().toUtf8().data());
    }
}
