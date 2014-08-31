#include "decodrdbusctrl.h"
#include <QtCore/QTimer>

DecodrDbusCtrl::DecodrDbusCtrl(QObject *parent)
: QObject(parent)
, mHeartbeatTimer(new QTimer(this))
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
    mHeartbeatTimer->start();
}

