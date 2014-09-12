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

struct RejectedClusterInfo
{
    int clusterNo;
    int blockBegin; //< offset of @property pixels
    QList<int> pixels; //< pixels block by block { (b0x0, b0y0), (b0x1, b0y1),.. (b0x7, b0y7),.. (bNx7, bNy6), (bNx7, bNy7) }
    
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
    static BitmapInfo processBitmap(const QByteArray &bm, QList<int> &jpegHeads, QList<int> &goodHeads);
    
};


Q_DECLARE_METATYPE(DecodedClusterInfo)
Q_DECLARE_METATYPE(DecodedClusters)
Q_DECLARE_METATYPE(RejectedClusterInfo)
Q_DECLARE_METATYPE(RejectedClusters)
Q_DECLARE_METATYPE(Pixmap)
Q_DECLARE_METATYPE(Result)
Q_DECLARE_METATYPE(BitmapInfo)

#endif
