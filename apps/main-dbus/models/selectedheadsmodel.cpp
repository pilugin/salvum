#include "selectedheadsmodel.h"
#include "workspacemodel.h"

#include <QPointer>

class SelectedHeadsModel::Private
{
public:
    struct Head
    {        
        int clusterNo;
        bool isGood;        
        
        struct WspaceData {
            WspaceData(): set(false) {}
            
            bool set;
            int clustersDecoded;
            int blocksDecoded;
            int blocksTotal;
        };
        mutable WspaceData wspaceData;
    };
    QList<Head> heads;
    
    QPointer<WorkspaceModel> wspaceModel;
};

SelectedHeadsModel::SelectedHeadsModel(QObject *parent)
: QAbstractListModel(parent)
, m_d(new Private)    
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif    
}

SelectedHeadsModel::~SelectedHeadsModel()
{
    delete m_d;
}

QHash<int, QByteArray> SelectedHeadsModel::roleNames_internal() const
{
    QHash<int, QByteArray> r;
    r.insert(Role_Cluster, "cluster");
    r.insert(Role_IsGood, "isGood");
    r.insert(Role_ClustersDecoded, "clustersDecoded");    
    r.insert(Role_BlocksDecoded, "blocksDecoded");
    r.insert(Role_BlocksTotal, "blocksTotal");
    return r;
}

int SelectedHeadsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_d->heads.size();
}

QModelIndex SelectedHeadsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (column!=0 || parent.isValid())
        return QModelIndex();
        
    return createIndex(row, 0, row);
}

QVariant SelectedHeadsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    const auto &v = m_d->heads[index.row()];    
    if ( (role==Role_ClustersDecoded || role==Role_BlocksDecoded || role==Role_BlocksTotal)
            && v.wspaceData.set==false
            && m_d->wspaceModel) {
            
        v.wspaceData.set = true;
        QModelIndex info =  m_d->wspaceModel->indexByCluster(v.clusterNo);
        if (info.isValid()) {
            v.wspaceData.clustersDecoded    = info.data(WorkspaceModel::Role_ClustersDecoded).toInt();
            v.wspaceData.blocksDecoded      = info.data(WorkspaceModel::Role_BlocksDecoded).toInt();
            v.wspaceData.blocksTotal        = info.data(WorkspaceModel::Role_BlocksTotal).toInt();
        }
    }
    switch (role) {
    case Role_Cluster:          return v.clusterNo;
    case Role_IsGood:           return v.isGood;    
    case Role_ClustersDecoded:  return v.wspaceData.set ? v.wspaceData.clustersDecoded  : 0;
    case Role_BlocksDecoded:    return v.wspaceData.set ? v.wspaceData.blocksDecoded    : 0;
    case Role_BlocksTotal:      return v.wspaceData.set ? v.wspaceData.blocksTotal      : 0;
    default:
        return QVariant();
    }        
}

void SelectedHeadsModel::setWorkspaceModel(WorkspaceModel *wspace)
{
    m_d->wspaceModel = wspace;
}
    
void SelectedHeadsModel::onHeadSelected(int clusterNo, bool isSelected)
{
    if (isSelected)
        appendHead(clusterNo, false);
    else 
        removeHead(clusterNo);
}

void SelectedHeadsModel::onGoodHeadSelected(int clusterNo, bool isSelected)
{
    if (isSelected)
        appendHead(clusterNo, true);
    else
        removeHead(clusterNo);
}
    
void SelectedHeadsModel::appendHead(int clusterNo, bool isGood)
{
    beginInsertRows(QModelIndex(), m_d->heads.size(), m_d->heads.size());
    m_d->heads.push_back(Private::Head() = { clusterNo, isGood, Private::Head::WspaceData() });
    endInsertRows();
    emit selectedHeadsUpdated();        
    
}

void SelectedHeadsModel::removeHead(int clusterNo)
{
    for (int i=0; i<m_d->heads.size(); ++i)
        if (m_d->heads[i].clusterNo == clusterNo) {
            beginRemoveRows(QModelIndex(), i, i);
            m_d->heads.removeAt(i);
            --i;
            endRemoveRows();
        }
    emit selectedHeadsUpdated();        
}

void SelectedHeadsModel::clear()
{
    beginResetModel();
    m_d->heads.clear();
    endResetModel();
    emit selectedHeadsUpdated();        
}

QList<int> SelectedHeadsModel::selectedHeads() const
{
    QList<int> r;
    for (const auto &h: m_d->heads)
        r.push_back(h.clusterNo);
    return r;
}
