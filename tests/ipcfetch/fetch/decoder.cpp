#include "decoder.h"

#include <if/ifetch.h>
#include <if/ilog.h>

#include <QtDebug>
#include <QtCore>

using namespace Log;

Decoder::Decoder()
: mLast(0), mCount(0), mFetch(0)
{
}

bool Decoder::restart(IFetch *f)
{
    QByteArray cluster;
    int clusterNo = IFetch::InvalidClusterNo;

    mFetch = f;

    for (int i=0; i<10 && clusterNo == IFetch::InvalidClusterNo; ++i)
        f->fetch(clusterNo, cluster);
    ++mCount;
    
    mUsedClusters.clear();
    mUsedClusters.append(clusterNo);

    if ((unsigned int)cluster.size() < sizeof(int))
        return false;
    mLast = last( cluster );

    qDebug()<<"DECODER::RESTART: "<<clusterNo<<mLast;

    return true;
}

bool Decoder::decodeCluster()
{
    mUsedClusters.clear();

    QByteArray cluster;
    int clusterNo;
    
    mFetch->fetch(clusterNo, cluster);
    mUsedClusters.append(clusterNo);
    if (clusterNo == IFetch::InvalidClusterNo || (unsigned int)cluster.size()<sizeof(int))
        return false;

    Msg("D:last=%d, cur=%d--%d", mLast, first(cluster), last(cluster));
    if (first(cluster) != mLast)
        return false;
    
    mLast = last(cluster);

    ++mCount;

    qDebug()<<"DECODER::DECOD[success]"<<mCount<<mLast;

    return true;
}

int Decoder::first(const QByteArray &ar) const
{
    return *reinterpret_cast<const int *>(ar.data());
}

int Decoder::last(const QByteArray &ar) const
{
    return *(reinterpret_cast<const int *>(ar.data() + ar.size()) -1);
}
