#include "decodrdbushub.h"
#include <QProcess>
#include <QtDebug>
#include <algorithm>

static const char *s_decodrExeName = "./salvum-decodr";

DecodrDbusHub::DecodrDbusHub(QObject *parent)
: Ui::QObjectListModel<DecoderDbusController>(parent)
, mWspace(nullptr)
{
    new DecodrHubAdaptor(this);

    if (!QDBusConnection::sessionBus().registerObject("/hub", this)) {
        qDebug()<<"Failed to register DecodrHub:/hub";
    }
}

DecodrDbusHub::~DecodrDbusHub()
{
}

const DecoderDbusController *DecodrDbusHub::decoderByClientId(int clientId) const
{
    for (const QObject *obj: objectList()) {
        if (const DecoderDbusController *ptr = qobject_cast<const DecoderDbusController *>(obj))
            if (ptr->clientId() == clientId)
                return ptr;
    }
    
    return nullptr;
}

QDBusObjectPath DecodrDbusHub::aquireClient(int clientId)
{
    qDebug()<<__FUNCTION__;

    if (const DecoderDbusController *ptr = decoderByClientId(clientId))
        return ptr->dbusObjectPath();
    else {
        QDBusObjectPath path(QString("/salvum/%1").arg(clientId));
        DecoderDbusController *object = new DecoderDbusController(clientId, path, this);

        connect(object, SIGNAL(connectedChanged(bool)), this, SLOT(decodrConnected(bool)) );

        if (!QDBusConnection::sessionBus().registerObject(path.path(), object)) {
            qDebug()<<"Failed to register DecodrCtrl:"<<path.path();
        }
    
        appendObject(object);

        return path;
    }
}

void DecodrDbusHub::releaseClient(int clientId)
{
    if (const DecoderDbusController *ptr = decoderByClientId(clientId))
        delete ptr;
}

void DecodrDbusHub::createDecoders(const QString &shmemPath, const QVariant &heads)
{
    QList<int> c;
    bool ok;
    for (QVariant cluster : heads.toList()) {
        int intc = cluster.toInt(&ok);
        if (ok)
            c.push_back(intc);
        else
            qDebug()<<"ERR: DecodrDbusHub::createDecoders. failed to cast to int " << heads;
    }
    createDecoders(shmemPath, c);
}

void DecodrDbusHub::createDecoders(const QString &shmemPath, const QList<int> &heads)
{
    mShmemPath = shmemPath;
    mHeads.append(heads);
    
    for (int i=0; i<heads.size(); ++i)
        if (!QProcess::startDetached(s_decodrExeName)) 
            qDebug()<<"ERR: DecodrDbusHub::startDecoders: "<<strerror(errno);
}

void DecodrDbusHub::decodrConnected(bool connected)
{
    if (!connected)
        return;

    int c=0;
    int total=0;
    for (auto ptr_: objectList())
        if (DecoderDbusController *ptr = qobject_cast<DecoderDbusController *>(ptr_)) {
            ++total;
            if (ptr->connected())
                ++c;
        }
    if (c == total)
        emit allDecodersConnected();
}

void DecodrDbusHub::startProcessing()
{
    qDebug()<<"START PROC";
    for (auto ptr_ : objectList()) 
        if (DecoderDbusController *ptr = qobject_cast<DecoderDbusController *>(ptr_)) {
            if (ptr->started()) {
//            itr->second->sendResume();
                qDebug()<<"RESUMR";
            } else if (mHeads.size() >0) {
                qDebug()<<"START "<<mShmemPath<<mHeads.back();
                ptr->sendStart( mHeads.back(), mShmemPath, mWspace->getPathForDecoder(mHeads.back()) );
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

