#include "decodedclustersmodel.h"

DecodedClustersModel::DecodedClustersModel(QObject *parent)
: QAbstractListModel(parent)
{

#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif

}

QHash<int, QByteArray> DecodedClustersModel::roleNames_internal()
{
    static QHash<int, QByteArray> rn;
    if (rn.isEmpty()) {
        rn.insert(Role_ClusterNo, "clusterNo");
        rn.insert(Role_BlockBegin, "blockBegin");
        rn.insert(Role_BlockEnd, "blockEnd");
    }
    return rn;
}

QModelIndex DecodedClustersModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( parent.isValid() || column!=0 || row>=mDecodedClusters.size() || row<0 )
        return QModelIndex();
        
    return createIndex(row, column, row);
}

int DecodedClustersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mDecodedClusters.size();
}

QVariant DecodedClustersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
        
    const DecodedClusters::value_type &v = mDecodedClusters[index.row()];
    
    switch (role) {
    case Role_ClusterNo:
        return std::get<0>(v);
    case Role_BlockBegin:
        return std::get<1>(v);
    case Role_BlockEnd:
        return std::get<2>(v);
    default:
        return QVariant();
    }
}

void DecodedClustersModel::reset(const DecodedClusters &decodedClusters)
{
    beginResetModel();
    mDecodedClusters = decodedClusters;
    endResetModel();
}

void DecodedClustersModel::setCurrentCluster(int row)
{
    if (row < mDecodedClusters.size() && row>0) {
        emit currentClusterChanged(mCurrentCluster = row);
        const DecodedClusters::value_type &v = mDecodedClusters[row];
        emit currentClusterParamsChanged( std::get<0>(v), std::get<1>(v), std::get<2>(v) );
    }
}


