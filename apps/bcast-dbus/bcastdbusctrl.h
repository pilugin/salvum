#ifndef BCAST_DBUS_CTRL_H
#define BCAST_DBUS_CTRL_H

#include "org.salvum.BroadcastAdp.h"

class BcastDbusCtrl : public QObject
{
    Q_OBJECT
public:    
    static const char *shmemPath() { return "salv_broadcast"; }

    BcastDbusCtrl(QObject *parent =nullptr);
    ~BcastDbusCtrl();
    
    bool isValid() const;
    
public slots:
    Result setSource(const QString &mediaPath, const QString &bitmapPath);
    void start();
    void skip(int clusterNo, int length);
    void stop();
    Result saveBitmap(const QString &bitmapPath);    
    void dumpStats();
    void emitProgress();
    void emitBitmapProcessed();
    void quit();
    
signals:
    void progress(int currentClusterNo, int clustersCount);
    void bitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, const BitmapInfo &info);
    void quitted();
    
private:    
    class Private;
    Private *m_d;
};

#endif // BCAST_DBUS_CTRL_H