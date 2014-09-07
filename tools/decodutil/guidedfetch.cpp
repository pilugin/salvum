#include "guidedfetch.h"
#include <util/isettings.h>

#include <QtDebug>

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

void GuidedFetch::fetch(int &clusterNo, QByteArray &cluster)
{
    if (atEnd()) {
        emit end();
        return;
    }

    const qint64 clusterSize = Settings::Get(Settings::ClusterSize).toLongLong();

    int curClusterNo = mClusters.front();
    mClusters.pop_front();
    if (!mFile.seek( clusterSize * curClusterNo )) {
        qDebug() << "Failed to seek to " << cluster;
        clusterNo = InvalidClusterNo;
        return;
    }

    clusterNo = curClusterNo;
    cluster = mFile.read( clusterSize );
    
    emit fetched( clusterNo );
}

bool GuidedFetch::rewind(int)
{
    return !atEnd();
}
