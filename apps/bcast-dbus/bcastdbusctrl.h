#ifndef BCAST_DBUS_CTRL_H
#define BCAST_DBUS_CTRL_H

#include "org.salvum.BroadcastAdp.h"

class BcastDbusCtrl : public QObject
{
    Q_OBJECT
public:    
    static const char *shmemPath() { return "salv_broadcast"; }

    BcastDbusCtrl(const char *mediaPath, const char *bitmapPath);
    ~BcastDbusCtrl();
    
    bool isValid() const;
    
public slots:
    void start();
    void skip(int clusterNo, int length);
    void stop();
    void emitProgress();
    
signals:
    void progress(int currentClusterNo, int clustersCount);
    void stopped();
    
private:    
    class Private;
    Private *m_d;
};

#endif // BCAST_DBUS_CTRL_H