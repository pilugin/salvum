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

typedef QList<DecodedClusterInfo> DecodedClusters;

struct RejectedClusterInfo
{
    int clusterNo;
    int blockBegin; //< offset of @property pixels
    QList<int> pixels; //< pixels block by block { (b0x0, b0y0), (b0x1, b0y0),.. (b0x7, b0y7),.. (bNx7, bNy6), (bNx7, bNy7) }
    
    QList<int>::const_iterator block(int blockNumber) const;
    int pixel(int blockNumber, int x, int y) const;
};

typedef QList<RejectedClusterInfo> RejectedClusters;

struct Pixmap
{
    int width;
    int height;
    QList<int> pixels;
};

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
Q_DECLARE_METATYPE(Common::DecodedClusters)
Q_DECLARE_METATYPE(Common::RejectedClusterInfo)
Q_DECLARE_METATYPE(Common::RejectedClusters)
Q_DECLARE_METATYPE(Common::Pixmap)
Q_DECLARE_METATYPE(Common::ImageInfo)
Q_DECLARE_METATYPE(Common::Result)
Q_DECLARE_METATYPE(Common::BitmapInfo)

#endif
