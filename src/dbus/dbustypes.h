#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QtCore/QList>
#include <QMetaType>

struct DecodedClusterInfo
{
    int clusterNo;
    int blockBegin;
    int blockEnd;
};

typedef QList<DecodedClusterInfo> DecodedClusters;

struct Pixmap
{
    int width;
    int height;
    QList<int> pixels;
};

Q_DECLARE_METATYPE(DecodedClusterInfo)
Q_DECLARE_METATYPE(DecodedClusters)
Q_DECLARE_METATYPE(Pixmap)


#endif
