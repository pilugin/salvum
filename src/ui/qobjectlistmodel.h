#ifndef QOBJECT_LIST_MODEL_H
#define QOBJECT_LIST_MODEL_H

#include <QAbstractListModel>

namespace Ui {

class QObjectListModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    QObjectListModelBase(const QMetaObject &mo, QObject *parent =nullptr);
    ~QObjectListModelBase();
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent =QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;        
    
    virtual const QObject *object(const QModelIndex &index) const;
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif    

public slots:
    void appendObject(QObject *object);
    void removeObject(QObject *object);
    void eraseObject(int row);
    void clear();

protected:
    const QObjectList &objectList() const;
    static int firstUserRole(const QMetaObject &mo);

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
        : QObjectListModelBase(Object::staticMetaObject, parent) {}

    const Object *object(const QModelIndex &index) const 
        { return qobject_cast<const Object *>(QObjectListModelBase::object(index) ); }

protected:
    static int firstUserRole()
        { return QObjectListModelBase::firstUserRole(Object::staticMetaObject); }
};

}

#endif // QOBJECT_LIST_MODEL_H
