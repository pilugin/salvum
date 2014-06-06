#include "decoder.h"

#include <if/ifetch.h>

#include <QtDebug>
#include <QtCore>

Decoder::Decoder(int size) : mLast(0), mCount(0), mTotal(size), mFetch(0) 
{
}

bool Decoder::restart(IFetch *f)
{
    QByteArray cluster;
    int clusterNo;

    mFetch = f;

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

    if (first(cluster) != mLast)
        return false;
    
    mLast = last(cluster);

    ++mCount;

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
