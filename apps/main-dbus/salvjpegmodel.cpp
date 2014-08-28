#include "salvjpegmodel.h"
#include "salvjpegobject.h"

SalvJpegModel::SalvJpegModel(QObject *parent)
: QAbstractListModel(parent)
{

#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif

}

QHash<int, QByteArray> SalvJpegModel::roleNames_internal()
{
    static QHash<int, QByteArray> rn;
    if (rn.isEmpty()) {
        rn.insert(Role_Id, "id");
        rn.insert(Role_InProgress, "inProgress");
        rn.insert(Role_Complete, "complete");
        rn.insert(Role_Image, "image");
        rn.insert(Role_Shade, "shade");
        rn.insert(Role_DecodedClusters, "decodedClusters");
    }
    return rn;
}

QModelIndex SalvJpegModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( parent.isValid() || column!=0 || row>=mList.size() || row<0 )
        return QModelIndex();
	
    return createIndex(row, column, row);
}

int SalvJpegModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mList.size();
}

QVariant SalvJpegModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
	
    SalvJpegObject *object = mList[index.row()];
    
    switch (role) {
    case Role_Id:
        return object->id();
    case Role_InProgress:
        return object->inProgress();
    case Role_Complete:
        return object->complete();
    case Role_Image:
        return object->imageId();
    case Role_Shade:
        return object->shadeId();
    case Role_DecodedClusters:
        return QVariant::fromValue(object->decodedClusters());
    default:
        return QVariant();
    }
}
    
void SalvJpegModel::decodrClientAdded(int clientId, QDBusObjectPath, DecodrDbusCtrl *client)
{
    SalvJpegObject *o = new SalvJpegObject(clientId, imageProviderPrefix(), this);
    
    connect(client, SIGNAL(resume()),   o, SLOT(decodrInProgress()) );
    connect(client, SIGNAL(start(int)), o, SLOT(decodrInProgress()) );
    connect(client, SIGNAL(atEndRecv(bool,DecodedClusters,Pixmap)), 
                o,  SLOT(decodrAtEnd(bool,DecodedClusters,Pixmap))  );
    
    connect(o, SIGNAL(inProgressChanged(bool)), this, SLOT(itemUpdated()) );
    
    
    beginInsertRows(QModelIndex(), mList.size(), mList.size());
    mList.push_back(o);
    endInsertRows();
}

void SalvJpegModel::decodrClientReleased(int clientId, QDBusObjectPath)
{
    int i=0;
    for (auto itr=mList.begin(); itr != mList.end(); ++i, ++itr) 
        if ((*itr)->id() == clientId) {            
            beginRemoveRows(QModelIndex(), i, i);
            delete *itr;
            mList.erase(itr);
            endRemoveRows();
            break;
        }
    
}

void SalvJpegModel::itemUpdated()
{
    QObject *o = sender();
    for (int i=0; i<mList.size(); ++i) 
        if (o == mList[i]) {
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx);
            break;
        }
}
    
QString SalvJpegModel::imageProviderName()
{
    return "salv-image";
}

QString SalvJpegModel::imageProviderPrefix()
{
    return QString("image://%1").arg(imageProviderName());
}
