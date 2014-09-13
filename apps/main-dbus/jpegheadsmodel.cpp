#include "jpegheadsmodel.h"
#include <QList>
#include <QVector>

class JpegHeadsModel::Private
{
public:
    enum { ClusterRole, SelectedRole };
    struct Item
    {
        int cluster;
        bool selected;        
    };
    
    QList<Item> items;
};

JpegHeadsModel::JpegHeadsModel(QObject *parent)
: QAbstractListModel(parent)
, m_d(new Private)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif
} 

JpegHeadsModel::~JpegHeadsModel()
{
    delete m_d;
}

QHash<int, QByteArray> JpegHeadsModel::roleNames_internal()
{
    QHash<int, QByteArray> r;
    r.insert(Private::ClusterRole, "cluster");
    r.insert(Private::SelectedRole, "selected");
    return r;
}

QModelIndex JpegHeadsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row <0 || row>=m_d->items.size() || column!=0 || parent.isValid())
        return QModelIndex();
        
    return createIndex(row, 0, row);
}

int JpegHeadsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())   
        return 0;
        
    return m_d->items.size();
}

QVariant JpegHeadsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    else switch (role) {
    case Private::ClusterRole:  return m_d->items[index.row()].cluster;
    case Private::SelectedRole: return m_d->items[index.row()].selected;
    default:
        return QVariant();
    }
}

void JpegHeadsModel::setHeads(const QList<int> &heads)
{
    beginResetModel();
    m_d->items.clear();
    for (int c: heads) {
        Private::Item i = { c, false };
        m_d->items.push_back(i);
    }
    endResetModel();
}

void JpegHeadsModel::toggleSelected(int cluster)
{
    int index_=0;
    for (; index_<m_d->items.size(); ++index_)
        if (m_d->items[index_].cluster == cluster)
            break;

    if (index_>=m_d->items.size())
        return;
        
    m_d->items[index_].selected = ! m_d->items[index_].selected;
    emit headSelected(m_d->items[index_].cluster, m_d->items[index_].selected);
    emit dataChanged(index(index_), index(index_));
}
