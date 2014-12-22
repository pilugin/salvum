#include "salvdbuscheck.h"
#include <jpeg/imagehelpers.h>
#include <jpeg/picojpegdecodr.h>
#include <util/ilog.h>

#include <QtCore/QEventLoop>
#include <QtDebug>

using namespace Log;

class SalvDbusCheck::Private
{
public:
    explicit Private(QObject *parent)
    : eventLoop(new QEventLoop(parent))
    {
    }
    
    QEventLoop *eventLoop;    
    int baselineClusterNo;
};

////////////////////////////

SalvDbusCheck::SalvDbusCheck(QObject *parent)
: QObject(parent)
, m_d(new Private(this))
{
}

SalvDbusCheck::~SalvDbusCheck()
{
    delete m_d;    
}
    
void SalvDbusCheck::baseline(int clusterNo)
{
    m_d->baselineClusterNo = clusterNo;
    breakEventLoop(0);
}

void SalvDbusCheck::breakEventLoop(int retcod)
{
    m_d->eventLoop->exit(retcod);
}

bool SalvDbusCheck::isWaiting() const
{
    return m_d->eventLoop->isRunning();
}



SalvDbusCheck::SelectResult SalvDbusCheck::select(const Core3::Archive<Jpeg::DecodrState> &archive)
{
    Msg("\nCHECK::CHOOSE_BASELINE");

    // sending atEnd by DBus
    emit atEnd();

    // wait baseline or interrupt

    m_d->baselineClusterNo = -1;
    qDebug("~~~STARTING NESTED EVENTLOOP");
    int rc = m_d->eventLoop->exec();
    qDebug("~~~Exit NESTED EVENTLOOP: retcod=%d; baselineCluster=%08X", rc, m_d->baselineClusterNo);

    // check baseline; exit if -1
    if (m_d->baselineClusterNo != -1) {

        // find appropriate frame
        for (const auto &cs : archive.pendingClusters()) {
            if (m_d->baselineClusterNo == cs.cluster.first)
                return SelectResult(cs.cluster.first, cs.decodrState);

        }
    }

    return SelectResult(Common::InvalidClusterNo, Jpeg::DecodrState());
}
