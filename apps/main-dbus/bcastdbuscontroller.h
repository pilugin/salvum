#ifndef BCAST_DBUS_CLIENT_H
#define BCAST_DBUS_CLIENT_H

#include "org.salvum.Broadcast.h"

class BcastDbusController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentCluster   READ currentCluster NOTIFY progress)
    Q_PROPERTY(int clustersCount    READ clustersCount  NOTIFY progress)
public:
    BcastDbusController(QObject *parent =nullptr);
    
    int currentCluster() const { return mCurrentCluster; }
    int clustersCount() const { return mClustersCount; }
    
signals:
    void bitmapInfoUpdated(const Common::BitmapInfo &info);    
    void jpegHeadsUpdated(const QList<int> &jpegHeads);
    void goodHeadsUpdated(const QList<int> &goodHeads);
    void progress();
    
public slots:
    void setSource(const QString &mediaPath, const QString &bitmapPath) { mBcast->setSource(mediaPath, bitmapPath); }
    void start() { mBcast->start(); }
    
private slots:    
    void onBitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, Common::BitmapInfo info);
    void onProgress(int currentCluster, int clustersCount);

private:
    org::salvum::Broadcast *mBcast;     
    int mCurrentCluster;
    int mClustersCount;
};

#endif // BCAST_DBUS_CLIENT_H