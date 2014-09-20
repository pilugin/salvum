#include <ui/qobjectlistmodel.h>
#include <QtCore/QMetaProperty>
#include <QtDebug>

namespace Ui {

class QObjectListModelBase::Private
{
public:
    Private(const QMetaObject &mo) : metaObject(mo)
    {
    
    }
    
    QHash<int, QByteArray> roleNames_internal() const
    {
        QHash<int, QByteArray> rn;
        for (int i=0; i<metaObject.propertyCount(); ++i) 
            rn.insert(i, metaObject.property(i).name());
        
//        qDebug()<<rn;
        return rn;
    }
    
    
    QMetaObject metaObject;
    QObjectList items;
};

////////////////////////////////////////////////

QObjectListModelBase::QObjectListModelBase(const QMetaObject &mo, QObject *parent)
: QAbstractListModel(parent)
, m_d(new Private(mo) )
{

#if QT_VERSION < 0x050000
    setRoleNames(m_d->roleNames_internal());
#endif    

}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> QObjectListModelBase::roleNames() const
{
    return m_d->roleNames_internal();
}
#endif

QObjectListModelBase::~QObjectListModelBase()
{
    delete m_d;
}
    
QModelIndex QObjectListModelBase::index(int row, int column, const QModelIndex &parent) const
{
    if (column!=0 || parent.isValid() || row<0 || row>=m_d->items.size())
        return QModelIndex();
    else
        return createIndex(row, 0, row);
}

int QObjectListModelBase::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_d->items.size();
    else 
        return 0;
}

QVariant QObjectListModelBase::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()  || role<0 || role>=m_d->metaObject.propertyCount())
        return QVariant();
        
    return m_d->items[index.row()]->property( m_d->metaObject.property(role).name() );
}
    


void QObjectListModelBase::appendObject(QObject *object)
{
    beginInsertRows(QModelIndex(), m_d->items.size(), m_d->items.size() );
    m_d->items.push_back(object);
    for (int i=0; i<m_d->metaObject.propertyCount(); ++i) {        
        if (m_d->metaObject.property(i).notifySignalIndex() == -1)
            continue;
            
        // dirty hack.. Rewrite it later    
        QByteArray signal(SIGNAL());
        signal += m_d->metaObject.property(i).notifySignal().signature();
        connect(object, signal.data(), this, SLOT(objectUpdated()), /*Qt::AutoConnection |*/ Qt::UniqueConnection);        
    }
    connect(object, SIGNAL(destroyed(QObject*)), this, SLOT(removeObject(QObject*)) );
    
    endInsertRows();
}

void QObjectListModelBase::removeObject(QObject *object)
{
    int row = m_d->items.indexOf(object);
    if (row != -1)
        eraseObject(row);
}

void QObjectListModelBase::eraseObject(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    disconnect(m_d->items[row], 0, this, 0);
    m_d->items.erase(m_d->items.begin() + row);
    endRemoveRows();
}

void QObjectListModelBase::objectUpdated()
{
    int row = m_d->items.indexOf( sender() );
    if (row == -1)
        return;
        
    emit dataChanged(index(row), index(row));
}

}
