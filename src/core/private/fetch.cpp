#include <core/fetch.h>

namespace Core {

Fetch::Fetch(QObject *parent) : QObject(parent)
{
}

void Fetch::fetch(int &clusterNo, QByteArray &cluster)
{
    if (atEnd()) {
        emit end();
        return;
    }

    clusterNo = InvalidClusterNo;
    cluster.clear();
    doFetch(clusterNo, cluster);

    if (clusterNo != InvalidClusterNo)
        emit fetched(clusterNo, cluster);

    if (atEnd() || clusterNo == InvalidClusterNo)
        emit end();
}

}
