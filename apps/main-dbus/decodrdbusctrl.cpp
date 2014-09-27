#include "decodrdbusctrl.h"
#include <QtCore/QTimer>

DecodrDbusCtrl::DecodrDbusCtrl(QObject *parent)
: QObject(parent)
, mHeartbeatTimer(new QTimer(this))
, mCluster(0)
, mClustersDecoded(0)
, mBlocksDecoded(0)
, mBlocksTotal(0)
, mStatus(Initial)
{
    new DecodrCtrlAdaptor(this);
    
    mHeartbeatTimer->setSingleShot(true);
    mHeartbeatTimer->setInterval(5000);
    connect(mHeartbeatTimer, SIGNAL(timeout()), this, SIGNAL(noHeartbeat()) );
    mHeartbeatTimer->start();
    
    connect(this, SIGNAL(noHeartbeat()), this, SLOT(onNoHeartbeat()) );
}

DecodrDbusCtrl::~DecodrDbusCtrl()
{
}

void DecodrDbusCtrl::heartbeat()
{
    if (mStatus == Initial) {
        mStatus = Connected;
        emit statusChanged();
        emit connected();
    }
    mHeartbeatTimer->start();
}

void DecodrDbusCtrl::sendStart(int clusterNo, const QString &shmemPath)
{
    if (mStatus == Connected) {
        mStatus = Started;
        mCluster = clusterNo;
        emit statusChanged();
    }
    emit start(clusterNo, shmemPath);
}

void DecodrDbusCtrl::progress(int clustersDecoded, int blocksDecoded, int blocksTotal)
{
    mClustersDecoded = clustersDecoded;
    mBlocksDecoded = blocksDecoded;
    mBlocksTotal = blocksTotal;
    emit progressChanged();
    
    mStatus = Decoding;
    emit statusChanged();
}

void DecodrDbusCtrl::onNoHeartbeat()
{
    mStatus = Disconnected;
    emit statusChanged();
    emit disconnected();
}

void DecodrDbusCtrl::decodingEnd(bool success)
{
    mStatus = success ? DecodingSuccess : DecodingFailed;
    emit statusChanged();
    disconnect(mHeartbeatTimer, 0, this, 0);
}

QString DecodrDbusCtrl::status() const
{
    switch (mStatus) {
    case Initial:   return "initial";
    case Connected: return "connected";
    case Started:   return "started";
    case Decoding:  return "decoding";
    case Disconnected:      return "disconnected";
    case DecodingFailed:    return "decodingFailed";
    case DecodingSuccess:   return "decodingSuccess";
    default:    return QString();
        
    }
}
