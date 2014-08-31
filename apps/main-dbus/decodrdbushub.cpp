#include "decodrdbushub.h"
#include <QSignalMapper>
#include <QtDebug>


DecodrDbusHub::DecodrDbusHub(QObject *parent)
: QObject(parent)
, mHeartbeatMapper(new QSignalMapper(this))
{
    new DecodrHubAdaptor(this);

    qRegisterMetaType<DecodrDbusCtrl *>("DecodrDbusCtrl *");
    
    if (!QDBusConnection::sessionBus().registerObject("/hub", this)) {
        qDebug()<<"Failed to register DecodrHub:/hub";
    }
    
    connect(mHeartbeatMapper, SIGNAL(mapped(int)), this, SLOT(releaseClient(int)) );
}

DecodrDbusHub::~DecodrDbusHub()
{
}

QDBusObjectPath DecodrDbusHub::aquireClient(int clientId)
{
    auto itr = mClients.find(clientId);
    if (itr == mClients.end()) {
        QDBusObjectPath path(QString("/salvum/%1").arg(clientId));
        DecodrDbusCtrl *object = new DecodrDbusCtrl(this);

        if (!QDBusConnection::sessionBus().registerObject(path.path(), object)) {
            qDebug()<<"Failed to register DecodrCtrl:"<<path.path();
        }
    
        mClients.insert(clientId, qMakePair(path, object));
        emit decodrClientAdded(clientId, path, object);
        
        mHeartbeatMapper->setMapping(object, clientId);
        connect(object, SIGNAL(noHeartbeat()), mHeartbeatMapper, SLOT(map()) );

        return path;
    } else
        return itr->first;
}

void DecodrDbusHub::releaseClient(int clientId)
{
    auto itr = mClients.find(clientId);
    if (itr != mClients.end()) {
        emit decodrClientReleased(clientId, itr->first);

        delete itr->second;
        mClients.erase(itr);
    }
}