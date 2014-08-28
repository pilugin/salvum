#include "decodrdbusctrl.h"

DecodrDbusCtrl::DecodrDbusCtrl(QObject *parent)
: QObject(parent)
{
    new DecodrCtrlAdaptor(this);
}

DecodrDbusCtrl::~DecodrDbusCtrl()
{
}

