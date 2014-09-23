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
    Q_PROPERTY(QList<int> clientIds READ clientIds NOTIFY clientIdsChanged)
public:
    DecodrDbusHub(QObject *parent =nullptr);
    ~DecodrDbusHub();
    
    QList<int> clientIds() const { return mClients.keys(); } 
    
    
public slots:
    QDBusObjectPath aquireClient(int clientId);
    void releaseClient(int clientId);   
    
    void startDecoders(const QString &shmemPath, const QList<int> &heads);
    void startDecoders(const QString &shmemPath, const QVariant &heads);
    
    void startProcessing();
    
signals:
    void clientIdsChanged();
    
    void decodrClientAdded(int clientId, QDBusObjectPath clientPath, DecodrDbusCtrl *client);
    void decodrClientReleased(int clientId, QDBusObjectPath clientPath);
    
    void allDecodersConnected();
    
private slots:
    void decodrConnected();    
    
private:    
    QMap<int, QPair<QDBusObjectPath, DecodrDbusCtrl *>> mClients;
    QSignalMapper *mHeartbeatMapper;
    QList<int> mHeads;
    QString mShmemPath;
};

#include <ui/qobjectlistmodel.h>

class DecodrDbusCtrlModel : public Ui::QObjectListModel<DecodrDbusCtrl>
{
    Q_OBJECT
public:
    DecodrDbusCtrlModel(QObject *parent =nullptr) : Ui::QObjectListModel<DecodrDbusCtrl>(parent) {}
    
public slots:
    void decodrClientAdded(int, QDBusObjectPath, DecodrDbusCtrl *client) 
    {
        appendObject(client);
    }    
};

#endif