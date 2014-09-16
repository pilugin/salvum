#include "decodrdbusctrl.h"
#include <QtCore/QTimer>

DecodrDbusCtrl::DecodrDbusCtrl(QObject *parent)
: QObject(parent)
, mHeartbeatTimer(new QTimer(this))
, mConnected(false)
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
    if (!mConnected) {
        emit connected();
        mConnected = true;
    }
    mHeartbeatTimer->start();
}

