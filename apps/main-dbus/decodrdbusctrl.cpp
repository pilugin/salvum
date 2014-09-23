#include "decodrdbusctrl.h"
#include <QtCore/QTimer>

DecodrDbusCtrl::DecodrDbusCtrl(QObject *parent)
: QObject(parent)
, mHeartbeatTimer(new QTimer(this))
, mIsConnected(false)
, mIsStarted(false)
, mCluster(0)
, mClustersDecoded(0)
, mBlocksDecoded(0)
, mBlocksTotal(0)
{
    new DecodrCtrlAdaptor(this);
    
    mHeartbeatTimer->setSingleShot(true);
    mHeartbeatTimer->setInterval(5000);
    connect(mHeartbeatTimer, SIGNAL(timeout()), this, SIGNAL(noHeartbeat()) );
    mHeartbeatTimer->start();
}

DecodrDbusCtrl::~DecodrDbusCtrl()
{
}

void DecodrDbusCtrl::heartbeat()
{
    if (!mIsConnected) {
        mIsConnected = true;
        emit isConnectedChanged();
    }
    mHeartbeatTimer->start();
}

void DecodrDbusCtrl::sendStart(const QString &shmemPath, int clusterNo)
{
    if (!mIsStarted) {
        mIsStarted = true;
        mCluster = clusterNo;
        emit isStartedChanged();
    }
    emit start(shmemPath, clusterNo);
}

void DecodrDbusCtrl::progress(int clustersDecoded, int blocksDecoded, int blocksTotal)
{
    mClustersDecoded = clustersDecoded;
    mBlocksDecoded = blocksDecoded;
    mBlocksTotal = blocksTotal;
    emit progressChanged();
}