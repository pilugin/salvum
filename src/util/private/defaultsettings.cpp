#include "util/defaultsettings.h"
#include <QtGlobal>

using namespace Settings;

QVariant DefaultSettings::get(int field) const
{
    switch (field) {
    case ClusterSize:       return 4096;
    case GoodClusters:      return QByteArray("1+JT"); // except 0GgRrU
    case UsedCluster:       return 'U';
    case ZeroCluster:       return '0';
    case JpegHead:          return 'J';
    case GoodJpegHead:      return 'G';
    case GoodJpegCluster:   return 'g';
    
    default:                Q_ASSERT(false);
    }
    return QVariant();
}
