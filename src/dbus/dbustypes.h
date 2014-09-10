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

struct Result
{
    operator bool() const { return errorCode == 0; }
    
    int errorCode; //< 0 - success
    QString error;
};

struct BitmapInfo 
{
    int jpegHeads;
    int goodHeads;
    int zeros;
    int goods;
    int decodable;
    int starts; //< [i-1]!=1 && [i]==1    
};

Q_DECLARE_METATYPE(DecodedClusterInfo)
Q_DECLARE_METATYPE(DecodedClusters)
Q_DECLARE_METATYPE(Pixmap)
Q_DECLARE_METATYPE(Result)
Q_DECLARE_METATYPE(BitmapInfo)

#endif
