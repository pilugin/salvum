#ifndef BCAST_DBUS_CTRL_H
#define BCAST_DBUS_CTRL_H

#include "org.salvum.BroadcastAdp.h"

class BcastDbusCtrl : public QObject
{
    Q_OBJECT
public:    
    BcastDbusCtrl(const char *shmem);
    ~BcastDbusCtrl();
    
public slots:
    void start();
    void skip(int clusterNo, int length);
    void stop();
    void emitProgress();
    
signals:
    void progress(int currentClusterNo, int clustersCount);
    
private:    
    class Private;
    Private *m_d;
};

#endif // BCAST_DBUS_CTRL_H