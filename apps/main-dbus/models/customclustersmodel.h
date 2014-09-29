#ifndef CUSTOM_CLUSTERS_MODEL_H
#define CUSTOM_CLUSTERS_MODEL_H

#include <ui/decodedclustersmodel.h>

class CustomClustersModel : public Ui::DecodedClustersModel
{
public:
    CustomClustersModel(Ui::ImageProvider *imgProv, QObject *parent =nullptr);
    void setIcons(QString firstAcc, QString middleAcc, QString lastAccNextRej, QString rej, QString lastRej, QString lastAcc);
    
    QVariant data(const QModelIndex &index, int role) const;
protected:
    enum {
        Role_Icon = DecodedClustersModel::Role_Custom,        
        
        Role_Custom
    };

    QHash<int, QByteArray> roleNames_internal() const;
    
    struct {
        QString firstAcc, middleAcc, lastAccNextRej, rej, lastRej, lastAcc;
    } mIcons;
};

#endif // CUSTOM_CLUSTERS_MODEL_H