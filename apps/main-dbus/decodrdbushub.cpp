#include "decodrdbushub.h"
#include <QSignalMapper>
#include <QProcess>
#include <QtDebug>

static const char *s_decodrExeName = "./salvum-decodr";

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
    qDebug()<<__FUNCTION__;

    auto itr = mClients.find(clientId);
    if (itr == mClients.end()) {
        QDBusObjectPath path(QString("/salvum/%1").arg(clientId));
        DecodrDbusCtrl *object = new DecodrDbusCtrl(this);

        if (!QDBusConnection::sessionBus().registerObject(path.path(), object)) {
            qDebug()<<"Failed to register DecodrCtrl:"<<path.path();
        }
    
        mClients.insert(clientId, qMakePair(path, object));
        emit decodrClientAdded(clientId, path, object);
        emit clientIdsChanged();
        
        mHeartbeatMapper->setMapping(object, clientId);
        connect(object, SIGNAL(noHeartbeat()), mHeartbeatMapper, SLOT(map()) );
        connect(object, SIGNAL(connected()), this, SLOT(decodrConnected()) );

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
        
        emit clientIdsChanged();
    }
}

void DecodrDbusHub::startDecoders(const QVariant &heads)
{
    QList<int> c;
    bool ok;
    for (QVariant cluster : heads.toList()) {
        int intc = cluster.toInt(&ok);
        if (ok)
            c.push_back(intc);
        else
            qDebug()<<"ERR: DecodrDbusHub::startDecoders. failed to cast to int " << heads;
    }
    startDecoders(c);
}

void DecodrDbusHub::startDecoders(const QList<int> &heads)
{
    mHeads.append(heads);
    
    for (int i=0; i<heads.size(); ++i)
        if (!QProcess::startDetached(s_decodrExeName)) 
            qDebug()<<"ERR: DecodrDbusHub::startDecoders: "<<strerror(errno);
}

void DecodrDbusHub::decodrConnected()
{
    qDebug()<<__FUNCTION__<<mHeads.size()<<mClients.size();
    
    DecodrDbusCtrl *dctrl = qobject_cast<DecodrDbusCtrl *>(sender());
    Q_ASSERT(dctrl);
    Q_ASSERT(mHeads.size() >0 );
    
    if (mHeads.size() == mClients.size())
        emit allDecodersConnected();
}

void DecodrDbusHub::startProcessing()
{
    if (mHeads.size() > 0) {
        Q_ASSERT(mHeads.size() >= mClients.size());
        
        for (auto itr=mClients.begin(); itr!=mClients.end(); ++itr) {
            itr->second->sendStart( mHeads.back() );
            mHeads.pop_back();
        }
    
    } 
}
