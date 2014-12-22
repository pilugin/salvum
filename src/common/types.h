#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QMetaType>

namespace Common {

enum { InvalidClusterNo =-1 };

typedef QPair<int, QByteArray> Cluster;

typedef QList<Cluster> Clusters;

struct DecodedClusterInfo
{
    int clusterNo;
    int blockBegin;
    int blockEnd;
};

typedef QList<int> Pixels;

struct ImageInfo
{
    int width;
    int height;
    QString imagePath;
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
    
    BitmapInfo();    
};

}

Q_DECLARE_METATYPE(Common::Cluster)
Q_DECLARE_METATYPE(Common::Clusters)
Q_DECLARE_METATYPE(Common::DecodedClusterInfo)
Q-DECLARE_METATYPE(Common::Pixels);
Q_DECLARE_METATYPE(Common::ImageInfo)
Q_DECLARE_METATYPE(Common::Result)
Q_DECLARE_METATYPE(Common::BitmapInfo)

#endif
