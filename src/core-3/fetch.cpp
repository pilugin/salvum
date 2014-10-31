#include <core-3/fetch.h>

namespace Core3 {

Fetch::~Fetch()
{
}

QByteArray Fetch::bitmap() const
{
    return QByteArray();
}

void Fetch::rewind(int /*clusterNo*/)
{
}

void Fetch::fastfwd()
{
}

void Fetch::skipClusters(int clusterNo, int len)
{
    QList<int> c;
    while (len --)
        c.push_back(clusterNo ++);

    skipClusters(c);
}

}
