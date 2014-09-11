#ifndef BCAST_DBUS_CLIENT_H
#define BCAST_DBUS_CLIENT_H

#include "org.salvum.Broadcast.h"

class BcastDbusController : public QObject
{
    Q_OBJECT
public:
    BcastDbusController(QObject *parent =nullptr);
    
private slots:    
    void onBitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, BitmapInfo info);
};

#endif // BCAST_DBUS_CLIENT_H