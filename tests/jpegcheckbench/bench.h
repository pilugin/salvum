#ifndef BENCH_H
#define BENCH_H

#include <QVector>
#include <QByteArray>

namespace Jpeg {
    class ICheck;
}

struct ClusterInput
{
    int clusterNo;
    QByteArray cluster;
    bool ok;
};

struct ClusterCheckInfo
{
    int clusterNo;
    double relevance;
    bool actuallyOk;
    bool expectedOk;
};

QVector<ClusterCheckInfo> runCheckDecod(const QVector<ClusterInput> &input, Jpeg::ICheck &checkr);


#endif