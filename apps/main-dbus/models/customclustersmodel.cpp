#include "customclustersmodel.h"

using namespace Ui;

CustomClustersModel::CustomClustersModel(QObject *parent)
: DecodedClustersModel(parent)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames_internal());
#endif

    setIcons(   "qrc://res/icons/clustersmodel/FA.png",
                "qrc://res/icons/clustersmodel/MA.png",
                "qrc://res/icons/clustersmodel/LA_R.png",
                "qrc://res/icons/clustersmodel/R.png",
                "qrc://res/icons/clustersmodel/LR.png",
                "qrc://res/icons/clustersmodel/LA.png" );
}

QHash<int, QByteArray> CustomClustersModel::roleNames_internal() const
{
    auto r = DecodedClustersModel::roleNames_internal();
    r.insert(Role_Icon, "icon");
    return r;
}

QVariant CustomClustersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
       
    int row = index.row();
//    const auto &v = mClusters[row];    
        
    switch (role) {
    case Role_Icon: {
            if (row == 0) // FIRST
                return mIcons.firstAcc;
                
            else if (row == (rowCount()-1))  // LAST
                return mClusters[row].rejected ? mIcons.lastRej : mIcons.lastAcc;
                
            // MIDDLE
            else if (mClusters[row].rejected) // REJ
                return mIcons.rej;
                
            else if (mClusters[row+1].rejected) // NEXT REJ
                return mIcons.lastAccNextRej;
                
            else                                    
                return mIcons.middleAcc;
        }
    default:
        return DecodedClustersModel::data(index, role);
    }
}

void CustomClustersModel::setIcons(QString firstAcc, QString middleAcc, QString lastAccNextRej, QString rej, QString lastRej, QString lastAcc)
{
    mIcons.firstAcc = firstAcc;
    mIcons.middleAcc = middleAcc;
    mIcons.lastAccNextRej = lastAccNextRej;
    mIcons.rej = rej;
    mIcons.lastRej = lastRej;
    mIcons.lastAcc = lastAcc;
    
    emit dataChanged(index(0), index(rowCount()-1));
}
