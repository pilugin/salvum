#ifndef SALVJPEGMODEL_H
#define SALVJPEGMODEL_H

#include <QAbstractListModel>
#include "decodrdbusctrl.h"
#include <ui/imageprovider.h>

namespace Ui {
class SalvJpegObject;
}

class SalvJpegModel : public QAbstractListModel, public Ui::ImageProviderAdaptor
{
    Q_OBJECT
    Q_PROPERTY(QObject *currentSalv READ currentSalv NOTIFY currentSalvChanged)
    Q_PROPERTY(int currentSalvIndex WRITE setCurrentSalvIndex)
public:
    
    SalvJpegModel(QObject *parent =nullptr);
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif

    Ui::SalvJpegObject *getSalvJpeg(int id) const;
    QObject *currentSalv() const;

    static QString imageProviderName();

    void setCurrentSalvIndex(int index);

    QImage get(const QString &image) const;

signals:
    void currentSalvChanged(QObject *salv);

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
        Role_ShadePath,
        Role_DecodedClusters,
    };
    static QHash<int, QByteArray> roleNames_internal();
    
    static QString imageProviderPrefix();
    
    QList<Ui::SalvJpegObject *> mList;
    int mCurrentSalvIndex;
};

#endif
