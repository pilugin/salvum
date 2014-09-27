#include "decodrdbushub.h"
#include <QSignalMapper>
#include <QProcess>
#include <QtDebug>
#include <algorithm>

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
    
//    connect(mHeartbeatMapper, SIGNAL(mapped(int)), this, SLOT(releaseClient(int)) );
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

void DecodrDbusHub::startDecoders(const QString &shmemPath, const QVariant &heads)
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
    startDecoders(shmemPath, c);
}

void DecodrDbusHub::startDecoders(const QString &shmemPath, const QList<int> &heads)
{
    mShmemPath = shmemPath;
    mHeads.append(heads);
    
    for (int i=0; i<heads.size(); ++i)
        if (!QProcess::startDetached(s_decodrExeName)) 
            qDebug()<<"ERR: DecodrDbusHub::startDecoders: "<<strerror(errno);
}

void DecodrDbusHub::decodrConnected()
{
    int c=0;
    for (const auto &p: mClients)
        if (p.second->isConnected())
            ++c;
    if (c == mClients.size())
        emit allDecodersConnected();
}

void DecodrDbusHub::startProcessing()
{
    qDebug()<<"START PROC";
    for (auto itr=mClients.begin(); itr!=mClients.end(); ++itr) {
        if (itr->second->isStarted()) {
//            itr->second->sendResume();
            qDebug()<<"RESUMR";
        } else if (mHeads.size() >0) {
            qDebug()<<"START "<<mShmemPath<<mHeads.back();
            itr->second->sendStart( mHeads.back(), mShmemPath );
            mHeads.pop_back();
        }
    }
}

int DecodrDbusHub::getRewindCluster() const
{
    qDebug()<<mHeads;
    auto min = std::min_element(mHeads.begin(), mHeads.end());
    return min==mHeads.end() ? -1 : *min;
}

