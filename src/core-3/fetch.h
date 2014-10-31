#ifndef CORE_3_FETCH_H
#define CORE_3_FETCH_H

#include <common/types.h>
#include <QtCore/QList>
#include <QtCore/QByteArray>

namespace Core3 {

class Fetch
{
public:
    virtual ~Fetch();

    virtual bool atEnd() const =0;
    virtual QByteArray bitmap() const;

    const Common::Cluster &fetch()              { mLastFetched=doFetch(); return mLastFetched; }
    virtual void rewind(int clusterNo =Common::InvalidClusterNo);
    virtual void fastfwd();

    void skipClusters(int clusterNo, int len);
    virtual void skipClusters(const QList<int> &clusters) =0;

    const Common::Cluster &lastFetched() const  { return mLastFetched; }
protected:
    virtual Common::Cluster &doFetch() const =0;

    Common::Cluster mLastFetched;
};

}

#endif // CORE_3_FETCH_H