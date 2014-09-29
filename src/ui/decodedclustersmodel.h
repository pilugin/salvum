#ifndef DECODEDCLUSTERSMODEL_H
#define DECODEDCLUSTERSMODEL_H

#include <common/types.h>
#include <ui/rect.h>
#include <QAbstractListModel>
#include <QImage>

namespace Ui {

class DecodedClustersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int              currentCluster  READ currentCluster WRITE setCurrentCluster NOTIFY currentClusterChanged)
    Q_PROPERTY(QObject*         shadeRect1      READ shadeRect1     CONSTANT)
    Q_PROPERTY(QObject*         shadeRect2      READ shadeRect2     CONSTANT)
    Q_PROPERTY(QImage           rejectedImage   READ rejectedImage                          NOTIFY rejectedImageChanged)
    Q_PROPERTY(int rejectedImageOffset          READ rejectedImageOffset                    NOTIFY rejectedImageChanged)
public:
    
    DecodedClustersModel(QObject *parent =nullptr);
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent =QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
        
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif
    
    int currentCluster() const { return mCurrentCluster; }
    
    Rect *shadeRect1() { return mShadeRect1; }
    Rect *shadeRect2() { return mShadeRect2; }   
    QImage rejectedImage() const { return mRejectedImage; }
    int rejectedImageOffset() const { return mRejectedImageOffset; }

public slots:
    void setCurrentCluster(int row);
    void baseline();
    void reset(const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::ImageInfo &imageInfo);

signals:
    void currentClusterChanged(int row);
    void currentClusterParamsChanged(int clusterNo, int blockBegin, int blockEnd);
    void baselineSelected(int clusterNo);
    void rejectedImageChanged();
    
protected:
    enum 
    {
        Role_ClusterNo,
        Role_BlockBegin,
        Role_BlockEnd,
        Role_IsRejected,
        Role_RejectedPixels,
        
        Role_Custom
    };
    virtual QHash<int, QByteArray> roleNames_internal() const;

    struct ClusterInfo
    {
        int clusterNo;
        int blockBegin;
        int blockEnd;  //< ignored if rejected==true
        bool rejected;
    };
    typedef QList<ClusterInfo> Clusters;
    Clusters mClusters;
    
    QMap<int /*clusterNo*/, Common::RejectedClusterInfo> mRejectedPieces;
    Common::ImageInfo mImageInfo;

    int mCurrentCluster;
    
    Rect *mShadeRect1, *mShadeRect2;
    QImage mRejectedImage;
    int mRejectedImageOffset;

    const ClusterInfo &currentClusterInfo() const { return mClusters[mCurrentCluster]; }
    Common::RejectedClusterInfo currentRejectedCluster() const;
    void clearRejectedImage();
};

}

#endif
