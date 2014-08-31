#ifndef DECODRDBUSHUB_H
#define DECODRDBUSHUB_H

#include "org.salvum.DecodrHubAdp.h"
#include "decodrdbusctrl.h"
#include <QPair>
#include <QMap>

class QSignalMapper;

Q_DECLARE_METATYPE(DecodrDbusCtrl *);

class DecodrDbusHub : public QObject
{
    Q_OBJECT
public:
    DecodrDbusHub(QObject *parent =nullptr);
    ~DecodrDbusHub();
    
public slots:
    QDBusObjectPath aquireClient(int clientId);
    void releaseClient(int clientId);
    
signals:
    void decodrClientAdded(int clientId, QDBusObjectPath clientPath, DecodrDbusCtrl *client);
    void decodrClientReleased(int clientId, QDBusObjectPath clientPath);
    
private:    
    QMap<int, QPair<QDBusObjectPath, DecodrDbusCtrl *>> mClients;
    QSignalMapper *mHeartbeatMapper;
};

#endif