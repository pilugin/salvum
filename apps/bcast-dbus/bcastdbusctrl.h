#ifndef BCAST_DBUS_CTRL_H
#define BCAST_DBUS_CTRL_H

#include "org.salvum.BroadcastAdp.h"

class BcastDbusCtrl : public QObject
{
    Q_OBJECT
public:    
    static const char *shmemPathPrefix() { return "salv_broadcast_"; }

    BcastDbusCtrl(QObject *parent =nullptr);
    ~BcastDbusCtrl();
    
    bool isValid() const;
    
public slots:
    Common::Result setSource(const QString &mediaPath, const QString &bitmapPath);
    void start(int clusterNo);
    void skip(int clusterNo, int length);
    void stop();
    Common::Result saveBitmap(const QString &bitmapPath);    
    void dumpStats();
    void emitProgress();
    void emitBitmapProcessed();
    void quit();
    
signals:
    void progress(int currentClusterNo, int clustersCount);
    void bitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, Common::BitmapInfo info);
    void quitted();
    void shmemCreated(const QString &path);
    
private:    
    class Private;
    Private *m_d;
};

#endif // BCAST_DBUS_CTRL_H