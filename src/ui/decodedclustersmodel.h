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
    Q_PROPERTY(const Ui::Rect*  shadeRect1      READ shadeRect1     CONSTANT)
    Q_PROPERTY(const Ui::Rect*  shadeRect2      READ shadeRect2     CONSTANT)
public:
    
    DecodedClustersModel(QObject *parent =nullptr);
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent =QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    void reset(const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const QImage &image);
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif
    
    int currentCluster() const { return mCurrentCluster; }
    
    const Rect *shadeRect1() const { return mShadeRect1; }
    const Rect *shadeRect2() const { return mShadeRect2; }

public slots:
    void setCurrentCluster(int row);
    void baseline();
    
signals:
    void currentClusterChanged(int row);
    void currentClusterParamsChanged(int clusterNo, int blockBegin, int blockEnd);
    void baselineSelected(int clusterNo);
    
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
    
    QMap<int /*clusterNo*/, QList<int>/*pixels; order @see dbustypes.h*/> mRejectedPieces;
    QImage mImage;

    int mCurrentCluster;
    
    Rect *mShadeRect1, *mShadeRect2;
};

}

#endif