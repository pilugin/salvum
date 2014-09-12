#ifndef BCAST_DBUS_CLIENT_H
#define BCAST_DBUS_CLIENT_H

#include "org.salvum.Broadcast.h"

class BcastDbusController : public QObject
{
    Q_OBJECT
public:
    BcastDbusController(QObject *parent =nullptr);
    
signals:
    void bitmapInfoUpdated(const BitmapInfo &info);    
    void jpegHeadsUpdated(const QList<int> &jpegHeads);
    void goodHeadsUpdated(const QList<int> &goodHeads);
    
public slots:
    void setSource(const QString &mediaPath, const QString &bitmapPath);
    
private slots:    
    void onBitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, BitmapInfo info);

private:
    org::salvum::Broadcast *mBcast;     
};

#endif // BCAST_DBUS_CLIENT_H