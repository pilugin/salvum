#ifndef DECODEDCLUSTERSMODEL_H
#define DECODEDCLUSTERSMODEL_H

#include <dbus/dbustypes.h>
#include <QAbstractListModel>

class DecodedClustersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentCluster READ currentCluster WRITE setCurrentCluster NOTIFY currentClusterChanged)
public:
    
    DecodedClustersModel(QObject *parent =nullptr);
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    void reset(const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters);
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif
    
    int currentCluster() const { return mCurrentCluster; }

public slots:
    void setCurrentCluster(int row);
    void baseline();
    
signals:
    void currentClusterChanged(int row);
    void currentClusterParamsChanged(int clusterNo, int blockBegin, int blockEnd);
    void baselineSelected(int clusterNo);
    
private:
    enum 
    {
        Role_ClusterNo,
        Role_BlockBegin,
        Role_BlockEnd,
    };
    static QHash<int, QByteArray> roleNames_internal();

    DecodedClusters mDecodedClusters;    
    int mCurrentCluster;
};

#endif