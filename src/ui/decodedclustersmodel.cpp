#include <ui/decodedclustersmodel.h>
#include <jpeg/imagehelpers.h>

using namespace Common;

namespace Ui {

DecodedClustersModel::DecodedClustersModel(QObject *parent)
: QAbstractListModel(parent)
, mCurrentCluster(-1)
, mShadeRect1(new Rect(this))
, mShadeRect2(new Rect(this))
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

void DecodedClustersModel::reset(const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap)
{
    reset(decodedClusters, rejectedClusters, Jpeg::image(pixmap));
}

void DecodedClustersModel::reset(const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters, const QImage &image)
{
    beginResetModel();
    mClusters.clear(); 
    mImage = image;
    
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

    mShadeRect1->reset();
    mShadeRect2->reset();

    endResetModel();
}

void DecodedClustersModel::setCurrentCluster(int row)
{
    if (row < mClusters.size() && row>0) {
        emit currentClusterChanged(mCurrentCluster = row);
        const Clusters::value_type &v = mClusters[row];
        emit currentClusterParamsChanged( v.clusterNo, v.blockBegin, v.blockEnd );
        
        auto rects = Jpeg::shade( mImage, v.blockEnd );
        mShadeRect1->setX(rects.first.x());
        mShadeRect1->setY(rects.first.y());
        mShadeRect1->setWidth(rects.first.width());
        mShadeRect1->setHeight(rects.first.height());
        mShadeRect2->setX(rects.second.x());
        mShadeRect2->setY(rects.second.y());
        mShadeRect2->setWidth(rects.second.width());
        mShadeRect2->setHeight(rects.second.height());
    }
}

void DecodedClustersModel::baseline()
{
    if ( mCurrentCluster>=0 && mCurrentCluster<mClusters.size() ) {
        emit baselineSelected( mClusters[mCurrentCluster].clusterNo );
    }
}

}
