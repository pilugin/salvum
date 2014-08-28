#include "salvjpegobject.h"
#include "jpeg/imagehelpers.h"

SalvJpegObject::SalvJpegObject(int id_, const QString &imageProviderPrefix, QObject *parent)
: QObject(parent)
, mId(id_)
, mImageProviderPrefix(imageProviderPrefix)
, mImageId(QString("%1/image/%2").arg(imageProviderPrefix).arg(id_))
, mShadeId(QString("%1/shade/%2/%3").arg(imageProviderPrefix).arg(id_).arg(0))
, mInProgress(false)
, mComplete(false)
{
}

void SalvJpegObject::decodrAtEnd(bool complete, const DecodedClusters &decodedClusters, const Pixmap &pixmap)
{
    emit inProgressChanged(mInProgress = false);
    emit completeChanged(mComplete = complete);
    
    mImage = Jpeg::image(pixmap);
    emit imageChanged(mImageId);
}

void SalvJpegObject::decodrInProgress()
{
    emit inProgressChanged(mInProgress = true);
}

QObject *SalvJpegObject::decodedClusters() const
{
    return nullptr;
}