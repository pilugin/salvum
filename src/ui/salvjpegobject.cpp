#include <ui/salvjpegobject.h>
#include <ui/decodedclustersmodel.h>
#include <jpeg/imagehelpers.h>
#include <QtDebug>

using namespace Common;

namespace Ui {

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

void SalvJpegObject::fetchAtEnd(bool complete, const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters, const QImage &image)
{
    emit inProgressChanged(mInProgress = false);
    emit completeChanged(mComplete = complete);

    mImage = image;
    emit imageChanged(mImageId);
    emit shadeChanged(mShadeId);

    mSubmodel->reset(decodedClusters, rejectedClusters);
}

void SalvJpegObject::fetchAtEnd(bool complete, const DecodedClusters &decodedClusters, const RejectedClusters &rejectedClusters, const Pixmap &pixmap)
{
    fetchAtEnd(complete, decodedClusters, rejectedClusters, Jpeg::image(pixmap));
}

QString SalvJpegObject::imageId() const
{
    return mImage.isNull() ? QString() : mImageId; 
}

QString SalvJpegObject::shadeId() const
{
    return mImage.isNull() ? QString() : mShadeId;
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
    mShadeBlockEnd = blockEnd;
}

QImage SalvJpegObject::shade(int blockEnd) const
{
    if (blockEnd != mShade.blockEnd || mShade.image.isNull()) {
        mShade.blockEnd = blockEnd;
        mShade.image = Jpeg::highlight(mImage, blockEnd);
    }
    
    return mShade.image;
}

QList<int> SalvJpegObject::shadePath() const
{
    QPair<QRect,QRect> shade = Jpeg::shade(mImage, mShadeBlockEnd);
    
    QList<int> points = {
            shade.first.left(),     shade.first.top(),
            shade.first.right(),    shade.first.top(),
            shade.first.right(),    shade.first.bottom(),
            shade.second.left(),    shade.second.bottom(),
            shade.second.left(),    shade.second.top(),
            shade.second.right(),   shade.second.top()  
            };
    return points;
}

void SalvJpegObject::baselineSelected(int clusterNo)
{
    emit baseline(clusterNo);
    
    // prepare data for IFetch::skip call
    ;
}

}
