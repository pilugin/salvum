#include <util/guidedfetch.h>
#include <util/isettings.h>
#include <util/ilog.h>

#include <QtDebug>

using namespace Core;

GuidedFetch::GuidedFetch(const QString &filename, const QList<int> &clusters,QObject *parent) :
    Fetch(parent)
{
    mClusters = clusters;
    mFile.setFileName(filename);
    if (!mFile.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open file " << filename;
        return;
    }
}

bool GuidedFetch::atEnd() const
{
    return mClusters.empty() || !mFile.isOpen();
}

void GuidedFetch::doFetch(int &clusterNo, QByteArray &cluster)
{
    const qint64 clusterSize = Settings::Get(Settings::ClusterSize).toLongLong();

    int curClusterNo = mClusters.front();
    mClusters.pop_front();
    if (!mFile.seek( clusterSize * curClusterNo )) {
        qDebug() << "Failed to seek to " << cluster;
        return;
    }

    clusterNo = curClusterNo;
    cluster = mFile.read( clusterSize );

    Log::Msg("\n[F:%08X]", clusterNo);
}

bool GuidedFetch::rewind(int)
{
    return !atEnd();
}
