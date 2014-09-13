#include "selectedheadsmodel.h"

class SelectedHeadsModel::Private
{
public:
    struct Head
    {
        int clusterNo;
        bool isGood;
    };
    QList<Head> heads;
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
        
    switch (role) {
    case Role_Cluster:  return m_d->heads[index.row()].clusterNo;
    case Role_IsGood:   return m_d->heads[index.row()].isGood;
    default:
        return QVariant();
    }        
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
    m_d->heads.push_back(Private::Head() = { clusterNo, isGood });
    endInsertRows();
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
}

