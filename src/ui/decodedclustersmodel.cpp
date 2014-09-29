#include <ui/decodedclustersmodel.h>
#include <jpeg/imagehelpers.h>

using namespace Common;

namespace Ui {

DecodedClustersModel::DecodedClustersModel(ImageProvider *imgProv, QObject *parent)
: QAbstractListModel(parent)
, mCurrentCluster(-1)
, mShadeRect1(new Rect(this))
, mShadeRect2(new Rect(this))
, mRejectedImageProviderAdaptor(new RejectedImageProviderAdaptor(this))
{
    imgProv->addAdaptor( QString().sprintf("rejected-image-%p", this), mRejectedImageProviderAdaptor);   

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

void DecodedClustersModel::reset(const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters, const ImageInfo &imageInfo)
{
    beginResetModel();
    mClusters.clear(); 
    mImageInfo = imageInfo;
    
    auto dc_itr = decodedClusters.begin();
    auto rc_itr = rejectedClusters.begin();
    
    while (! (dc_itr==decodedClusters.end() && rc_itr==rejectedClusters.end())) {
        
        if (rc_itr==rejectedClusters.end()
            || (dc_itr!=decodedClusters.end() && (dc_itr->clusterNo < rc_itr->clusterNo))       ) {        
        
            mClusters.push_back( ClusterInfo() = { dc_itr->clusterNo, dc_itr->blockBegin, dc_itr->blockEnd, false } );
            ++dc_itr;
            
        } else {
        
            mClusters.push_back( ClusterInfo() = { rc_itr->clusterNo, rc_itr->blockBegin, 
                        rc_itr->blockBegin + (rc_itr->pixels.size()/8/8), true } );
            mRejectedPieces.insert( rc_itr->clusterNo, *rc_itr );
            ++rc_itr;
        }
    }
    
    mCurrentCluster = -1;    

    mShadeRect1->reset();
    mShadeRect2->reset();
    
    clearRejectedImage();

    endResetModel();
}

void DecodedClustersModel::setCurrentCluster(int row)
{
    if (row < mClusters.size() && row>0 && row!=mCurrentCluster) {
        emit currentClusterChanged(mCurrentCluster = row);
        const Clusters::value_type &v = mClusters[row];
        emit currentClusterParamsChanged( v.clusterNo, v.blockBegin, v.blockEnd );
        
        auto rects = Jpeg::shade( mImageInfo, v.blockEnd );
        mShadeRect1->setX(rects.first.x());
        mShadeRect1->setY(rects.first.y());
        mShadeRect1->setWidth(rects.first.width());
        mShadeRect1->setHeight(rects.first.height());
        mShadeRect2->setX(rects.second.x());
        mShadeRect2->setY(rects.second.y());
        mShadeRect2->setWidth(rects.second.width());
        mShadeRect2->setHeight(rects.second.height());
        
        // rejected cluster
        if (currentClusterInfo().rejected) {
            auto p = Jpeg::imageFragment(mImageInfo.width, mImageInfo.height, currentRejectedCluster());
            mRejectedImageProviderAdaptor->setImage(p.first, p.second);
        } else 
            clearRejectedImage();            
    }
}

void DecodedClustersModel::baseline()
{
    if ( mCurrentCluster>=0 && mCurrentCluster<mClusters.size() ) {
        emit baselineSelected( mClusters[mCurrentCluster].clusterNo );
    }
}

RejectedClusterInfo DecodedClustersModel::currentRejectedCluster() const
{
    const auto &v = currentClusterInfo();
    if (!v.rejected)
        return RejectedClusterInfo();
        
    return mRejectedPieces.value(v.clusterNo);        
}

void DecodedClustersModel::clearRejectedImage()
{
    mRejectedImageProviderAdaptor->setImage(QImage(), -1);
}

/////////////////////////////////////

RejectedImageProviderAdaptor::RejectedImageProviderAdaptor(QObject *parent)
: QObject(parent), mOffset(-1)
{
}

void RejectedImageProviderAdaptor::setImage(const QImage &image, int offset_)
{
    mImage = image;
    mOffset = offset_;
    emit updated();
}

}
