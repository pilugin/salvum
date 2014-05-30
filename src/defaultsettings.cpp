#include "defaultsettings.h"
#include <QtGlobal>

using namespace Settings;

QVariant DefaultSettings::get(int field) const
{
    switch (field) {
    case ClusterSize:       return 4096;
    case GoodClusters:      return QByteArray("1+");
    case UsedCluster:       return 'U';
    default:                Q_ASSERT(false);
    }
    return QVariant();
}
