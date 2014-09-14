#include "workspacemodel.h"

struct WorkspaceModel::Private
{
    struct HeadInfo
    {
        int cluster;
        QList<int> clusterList;
        int blocksDecoded;
        int blocksTotal;
        QImage preview;
    };
    QList<HeadInfo> items;
    QString wspacePath;
    QString bitmapPath;
    
    // non-datamodel stuff
    QString imageProviderName;
    
    void loadPreview(int index)
    {
        
    }
};

WorkspaceModel::WorkspaceModel(QObject *parent)
: QAbstractListModel(parent)
, m_d(new Private)
{
#if QT_VERSION >= 0x050000
    setRoleNames(roleNames_internal());
#endif    
}

WorkspaceModel::~WorkspaceModel()
{
    delete m_d;
}
    

int WorkspaceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())   
        return 0;
        
    return m_d->items.size();
}

QModelIndex WorkspaceModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row <0 || row>=m_d->items.size() || column!=0 || parent.isValid())
        return QModelIndex();
        
    return createIndex(row, 0, row);
}

QVariant WorkspaceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    else switch (role) {
    case Role_Cluster:      return m_d->items[index.row()].cluster;    
//    case Role_ClusterList:  return m_d->items[index.row()].clusterList;
    case Role_ClustersDecoded: return m_d->items[index.row()].clusterList.size();
    case Role_Preview: {
        m_d->loadPreview(index.row());
        return QString("image://%1/%2/preview-%3").arg(m_d->imageProviderName).arg(name()).arg(index.row());
    }
    case Role_BlocksDecoded:return m_d->items[index.row()].blocksDecoded;
    case Role_BlocksTotal:  return m_d->items[index.row()].blocksTotal;
    default: 
        return QVariant();
    };
}
    
QString WorkspaceModel::bitmapPath() const
{
    return m_d->bitmapPath;
}
    
QImage WorkspaceModel::get(const QString &image) const
{
}

void WorkspaceModel::loadWspace(const QString &path)
{
}

void WorkspaceModel::reloadWspace()
{
    loadWspace(m_d->wspacePath);
}
    
    
QHash<int, QByteArray> WorkspaceModel::roleNames_internal() const
{
    QHash<int, QByteArray> r;
    r.insert(Role_Cluster, "cluster");
    r.insert(Role_ClusterList, "clusterList");
    r.insert(Role_ClustersDecoded, "clustersDecoded");
    r.insert(Role_Preview, "preview");
    r.insert(Role_BlocksDecoded, "blocksDecoded");
    r.insert(Role_BlocksTotal, "blocksTotal");
    return r;
}

void WorkspaceModel::setImageProviderName(const QString &name)
{
    m_d->imageProviderName = name;
    emit dataChanged(index(0), index(m_d->items.size()));
}
