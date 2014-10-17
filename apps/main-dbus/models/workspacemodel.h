#ifndef WORKSPACE_MODEL_H
#define WORKSPACE_MODEL_H

#include <QAbstractListModel>
#include <ui/imageprovider.h>

class WorkspaceModel : public QAbstractListModel, public Ui::ImageProviderAdaptor
{
    Q_OBJECT
    Q_PROPERTY(QString bitmapPath   READ bitmapPath NOTIFY bitmapPathUpdated)
public:
    WorkspaceModel(QObject *parent =nullptr);
    ~WorkspaceModel();
    
    enum {
        Role_Cluster,
        Role_ClusterList,
        Role_ClustersDecoded,
        Role_Preview,
        Role_BlocksDecoded,
        Role_BlocksTotal
    };
    
    Q_INVOKABLE QString getPathForDecoder(int clusterNo) const;
    
    // Model stuff
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif    
    int rowCount(const QModelIndex &parent =QModelIndex()) const;
    QModelIndex index(int row, int column=0, const QModelIndex &parent =QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;   
    
    QModelIndex indexByCluster(int clusterNo) const;
    
    QString bitmapPath() const;
    
    QImage get(const QString &image) const;
    void setImageProviderName(const QString &name);
signals:
    void wspaceReady();
    void wspaceError(QString error);
    void bitmapPathUpdated();
    
public slots:
    void loadWspace(const QString &path);
    void createWspace(const QString &path, const QString &mediaPath);
    void reloadWspace();
    
private:
    
    virtual QHash<int, QByteArray> roleNames_internal() const;

    struct Private;
    Private *m_d;
};

#endif