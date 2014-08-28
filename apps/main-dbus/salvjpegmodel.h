#ifndef SALVJPEGMODEL_H
#define SALVJPEGMODEL_H

#include <QAbstractListModel>
#include "decodrdbusctrl.h"

class SalvJpegObject;

class SalvJpegModel : public QAbstractListModel
{
    Q_OBJECT
public:
    
    SalvJpegModel(QObject *parent =nullptr);
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif

public slots:
    void decodrClientAdded(int clientId, QDBusObjectPath, DecodrDbusCtrl *client);
    void decodrClientReleased(int clientId, QDBusObjectPath);
    
private slots:    
    void itemUpdated();
    
private:
    enum 
    {
        Role_Id,
        Role_InProgress,
        Role_Complete,
        Role_Image,
        Role_Shade,
        Role_DecodedClusters,
    };
    static QHash<int, QByteArray> roleNames_internal();
    
    static QString imageProviderName();
    static QString imageProviderPrefix();
    
    QList<SalvJpegObject *> mList;

};

#endif