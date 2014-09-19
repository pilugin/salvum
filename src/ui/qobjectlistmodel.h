#ifndef QOBJECT_LIST_MODEL_H
#define QOBJECT_LIST_MODEL_H

#include <QAbstractListModel>

class QObjectListModelBase : public QAbstractListModel
{
public:
    QObjectListModelBase(const QMetaObject &mo, QObject *parent =nullptr);
    ~QObjectListModelBase();
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;        
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif    

    void appendObject(QObject *object);
    void removeObject(QObject *object);
    void eraseObject(int row);

private slots:
    void objectUpdated();

private:
    class Private;
    Private *m_d;
};



template <class Object>
class QObjectListModel : public QObjectListModelBase
{
public:
    QObjectListModel<Object>(QObject *parent =nullptr) 
        : QObjectListModelBase(Object::getStaticMetaObject(), parent) {}
};


#endif // QOBJECT_LIST_MODEL_H