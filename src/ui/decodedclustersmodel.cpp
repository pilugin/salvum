#include <ui/decodedclustersmodel.h>

DecodedClustersModel::DecodedClustersModel(QObject *parent)
: QAbstractListModel(parent)
, mCurrentCluster(-1)
{

#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif

}

QHash<int, QByteArray> DecodedClustersModel::roleNames_internal() const
{
    static QHash<int, QByteArray> rn;
    if (rn.isEmpty()) {
        rn.insert(Role_ClusterNo, "clusterNo");
        rn.insert(Role_BlockBegin, "blockBegin");
        rn.insert(Role_BlockEnd, "blockEnd");
        rn.insert(Role_IsRejected, "isRejected");
        rn.insert(Role_RejectedPixels, "rejectedPixels");
    }
    return rn;
}

QModelIndex DecodedClustersModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( parent.isValid() || column!=0 || row>=mClusters.size() || row<0 )
        return QModelIndex();
        
    return createIndex(row, column, row);
}

int DecodedClustersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mClusters.size();
}

QVariant DecodedClustersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
        
    const Clusters::value_type &v = mClusters[index.row()];
    
    switch (role) {
    case Role_ClusterNo:
        return v.clusterNo;
    case Role_BlockBegin:
        return v.blockBegin;
    case Role_BlockEnd:
        return v.blockEnd;
    case Role_IsRejected:
        return v.rejected;
    case Role_RejectedPixels:
        return "role not implemented";
    default:
        return QVariant();
    }
}

void DecodedClustersModel::reset(const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters)
{
    beginResetModel();
    mClusters.clear(); 
    
    auto dc_itr = decodedClusters.begin();
    auto rc_itr = rejectedClusters.begin();
    
    while (! (dc_itr==decodedClusters.end() && rc_itr==rejectedClusters.end())) {
        
        if (rc_itr==rejectedClusters.end()
            || (dc_itr!=decodedClusters.end() && (dc_itr->clusterNo < rc_itr->clusterNo))       ) {        
        
            mClusters.push_back( ClusterInfo() = { dc_itr->clusterNo, dc_itr->blockBegin, dc_itr->blockEnd, false } );
            ++dc_itr;
            
        } else {
        
            mClusters.push_back( ClusterInfo() = { rc_itr->clusterNo, rc_itr->blockBegin, -1, true } );
            mRejectedPieces.insert( rc_itr->clusterNo, rc_itr->pixels );
            ++rc_itr;
        }
    }
    
    mCurrentCluster = -1;
    endResetModel();
}

void DecodedClustersModel::setCurrentCluster(int row)
{
    if (row < mClusters.size() && row>0) {
        emit currentClusterChanged(mCurrentCluster = row);
        const Clusters::value_type &v = mClusters[row];
        emit currentClusterParamsChanged( v.clusterNo, v.blockBegin, v.blockEnd );
    }
}

void DecodedClustersModel::baseline()
{
    if ( mCurrentCluster>=0 && mCurrentCluster<mClusters.size() ) {
        emit baselineSelected( mClusters[mCurrentCluster].clusterNo );
    }
}


