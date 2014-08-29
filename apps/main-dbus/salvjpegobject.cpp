#include "salvjpegobject.h"
#include "jpeg/imagehelpers.h"
#include "decodedclustersmodel.h"

SalvJpegObject::SalvJpegObject(int id_, const QString &imageProviderPrefix, QObject *parent)
: QObject(parent)
, mId(id_)
, mImageProviderPrefix(imageProviderPrefix)
, mImageId(QString("%1/image/%2").arg(imageProviderPrefix).arg(id_))
, mShadeId(QString("%1/shade/%2/%3").arg(imageProviderPrefix).arg(id_).arg(0))
, mInProgress(false)
, mComplete(false)
, mSubmodel(new DecodedClustersModel(this))
{
    connect(mSubmodel,  SIGNAL(currentClusterParamsChanged(int,int,int)),   this,   SLOT(currentClusterChanged(int,int,int)) );
    connect(mSubmodel,  SIGNAL(baselineSelected(int)),                      this,   SLOT(baselineSelected(int)) );
}

void SalvJpegObject::decodrAtEnd(bool complete, const DecodedClusters &decodedClusters, const Pixmap &pixmap)
{
    emit inProgressChanged(mInProgress = false);
    emit completeChanged(mComplete = complete);
    
    mImage = Jpeg::image(pixmap);
    emit imageChanged(mImageId);
    
    mSubmodel->reset(decodedClusters);
}

void SalvJpegObject::decodrInProgress()
{
    emit inProgressChanged(mInProgress = true);
}

QObject *SalvJpegObject::decodedClusters() const
{
    return mSubmodel;
}

void SalvJpegObject::currentClusterChanged(int clusterNo, int blockBegin, int blockEnd)
{
    emit shadeChanged(mShadeId = QString("%1/shade/%2/%3").arg(mImageProviderPrefix).arg(mId).arg(blockEnd) );
}

QImage SalvJpegObject::shade(int blockEnd) const
{
    if (blockEnd != mShade.blockEnd || mShade.image.isNull()) {
        mShade.blockEnd = blockEnd;
        mShade.image = Jpeg::highlight(mImage, blockEnd);
    }
    
    return mShade.image;
}

void SalvJpegObject::baselineSelected(int clusterNo)
{
    emit baseline(clusterNo);
    
    // prepare data for IFetch::skip call
    ;
}