#ifndef BCAST_DBUS_CLIENT_H
#define BCAST_DBUS_CLIENT_H

#include "org.salvum.Broadcast.h"
#include <ui/bitmapinfomodel.h>

class BcastDbusController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> jpegHeads READ jpegHeads NOTIFY bitmapUpdated)
    Q_PROPERTY(QList<int> goodHeads READ goodHeads NOTIFY bitmapUpdated)    
public:
    BcastDbusController(QObject *parent =nullptr);
    
    Q_INVOKABLE QObject *bitmapInfo() { return mBitmapInfo; }
    
    QList<int> jpegHeads() const { return mJpegHeads; }
    QList<int> goodHeads() const { return mGoodHeads; }
signals:
    void bitmapUpdated();    
    
public slots:
    void setSource(const QString &mediaPath, const QString &bitmapPath);
    
private slots:    
    void onBitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, BitmapInfo info);

private:
    BitmapInfoModel *mBitmapInfo; 
    org::salvum::Broadcast *mBcast;   
  
    QList<int> mJpegHeads;
    QList<int> mGoodHeads;  
};

#endif // BCAST_DBUS_CLIENT_H