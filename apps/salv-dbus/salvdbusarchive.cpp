#include "salvdbusarchive.h"

struct SalvDbusArchive::Private
{
    Private()
    : initialized(false)
    , initEmitted(false)
    , lastDecodedBlock(-1)
    {

    }

    bool initialized;
    bool initEmitted;
    int lastDecodedBlock;
};

SalvDbusArchive::SalvDbusArchive(QObject *parent)
: QObject(parent)
, m_d(new Private)
{
}

SalvDbusArchive::~SalvDbusArchive()
{
    delete m_d;
}

void SalvDbusArchive::onInitialized()
{
    m_d->initialized = true;
}

void SalvDbusArchive::onStateSet(const Common::Cluster &cluster, const Jpeg::DecodrState &decodrState, bool isOk)
{
    if (!m_d->initEmitted && m_d->initialized) {
        Common::ImageInfo ii;
        ii.width = decodrState.cursor.canvas()->width();
        ii.height = decodrState.cursor.canvas()->height();
        emit initialized(ii);
        m_d->initEmitted = true;
    }

    Common::DecodedClusterInfo dci;
    dci.blockBegin = m_d->lastDecodedBlock<0 ? 0 : m_d->lastDecodedBlock +1;
    dci.blockEnd = decodrState.cursor.currentBlockIndex() -1;
    dci.clusterNo = cluster.first;
    Common::Pixels pixels; // = Jpeg::getPixels( image, dci.blockBegin, dci.blockEnd );
    emit clusterDecoded(dci, pixels, isOk);

    if (isOk)
        m_d->lastDecodedBlock = decodrState.cursor.currentBlockIndex() -1;
}

void SalvDbusArchive::archiveBaselined(const Common::Cluster &cluster)
{

}
