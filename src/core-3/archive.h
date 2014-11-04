#ifndef CORE_3_ARCHIVE_H
#define CORE_3_ARCHIVE_H

#include <common/types.h>
#include <algorithm>

namespace Core3 {

template <class DecodrState>
class Archive
{
public:
    Archive();
    virtual ~Archive();

    void addNewCluster(const Common::Cluster &cluster);
    void setNewState(const DecodrState &decodrState, bool isOk);

    virtual QList<int> baseline(int clusterNo);

    // iface for Check
    struct ClusterStored
    {
        Common::Cluster cluster;
        bool isOk;
        bool decodrStateSet;
        DecodrState decodrState;
    };

    DecodrState lastOkState(ClusterStored *out =nullptr) const;

    QList<ClusterStored> pendingClusters() const { return mPendingClusters; }

protected:
    virtual void onInitialized();
    virtual void archiveBaselined(const Common::Cluster &cluster);

private:
    QList<ClusterStored> mPendingClusters;
    bool mInitialized;
};

//////////////////////////////////////////////////////////////

template <class DecodrState>
Archive<DecodrState>::Archive() : mInitialized(false)
{
}

template <class DecodrState>
Archive<DecodrState>::~Archive()
{
}

template <class DecodrState>
void Archive<DecodrState>::onInitialized()
{
}

template <class DecodrState>
void Archive<DecodrState>::archiveBaselined(const Common::Cluster &)
{
}

template <class DecodrState>
void Archive<DecodrState>::addNewCluster(const Common::Cluster &cluster)
{
    ClusterStored cs = { cluster, false, false, DecodrState() };
    mPendingClusters.push_back( cs );
}

template <class DecodrState>
void Archive<DecodrState>::setNewState(const DecodrState &decodrState, bool isOk)
{
    mPendingClusters.back().decodrState = decodrState;
    mPendingClusters.back().decodrStateSet = true;
    mPendingClusters.back().isOk = isOk;
}

template <class DecodrState>
DecodrState Archive<DecodrState>::lastOkState(ClusterStored *out) const
{
    int index = 0;
    for (int i=0, cnt=mPendingClusters.size(); i<cnt; ++i) {
        if (mPendingClusters[ cnt -i -1 ].isOk && mPendingClusters[ cnt -i -1 ].decodrStateSet) {
            index = cnt -i -1;
            break;
        }
    }
    if (out)
        *out = mPendingClusters[ index ];
    return mPendingClusters[ index ].decodrState;
}

template <class DecodrState>
QList<int> Archive<DecodrState>::baseline(int clusterNo)
{
    auto itr = std::find_if( mPendingClusters.constBegin(), mPendingClusters.constEnd(),
            [&](const ClusterStored &cs)->bool {
                return cs.cluster.first == clusterNo;
            }
        );

    if (itr == mPendingClusters.constEnd())
        return QList<int>();

    QList<int> rv;
    for (auto i=mPendingClusters.constBegin(); i<=itr; ++i) {
        rv.push_back( i->cluster.first );
        archiveBaselined( i->cluster );
    }
    mPendingClusters.clear();

    if (!mInitialized) {
        mInitialized = true;
        onInitialized();
    }

    return rv;
}


}

#endif // CORE_3_ARCHIVE_H
