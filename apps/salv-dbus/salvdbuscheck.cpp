#include "salvdbuscheck.h"

SalvDbusCheck::SalvDbusCheck(QObject *parent)
: Core::Check(parent)
, mEloop(new QEventLoop(this))
{
}
    
void SalvDbusCheck::baseline(int clusterNo)
{
}
    
const SalvDbusCheck::FrameDescription &SalvDbusCheck::chooseBaseline(const SalvDbusCheck::FrameDescription_v &frames)
{
}
