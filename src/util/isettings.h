#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QVariant>
#include <util/singleton.h>

class ISettings : public Singleton<ISettings>
{
public:
    virtual QVariant get(int field) const =0;
};

namespace Settings {

enum
{
    ClusterSize,
    GoodClusters,
    UsedCluster,
    ZeroCluster,
    JpegHead,
    GoodJpegHead,
    GoodJpegCluster,
};

inline QVariant Get(int field)
{
    return ISettings::instance()->get(field);
}

}

#endif
