#include "decoder.h"
#include "jpeg/imagehelpers.h"
#include <QtDebug>
#include <QDir>
#include <QImage>

using namespace Common;

Decoder::Decoder(org::salvum::DecodrCtrl *dbus, QObject *parent)
: QObject(parent)
, mDecodrCtrl(dbus)
{
    connect(dbus,SIGNAL(start(int,QString,QString)), this, SLOT(start(int,QString,QString)) );
    connect(dbus, SIGNAL(baseline(int)), this, SLOT(baseline(int)) );
    connect(dbus, SIGNAL(exit()), this, SLOT(exit()) );

    connect(this, SIGNAL(progress(int,int,int)), dbus, SLOT(progress(int,int,int)) );
    connect(this, SIGNAL(decodingEnd(bool)), dbus, SLOT(decodingEnd(bool)) );
    connect(this, SIGNAL(fetchAtEnd(bool,Common::DecodedClusters,Common::RejectedClusters,Common::ImageInfo)),
            dbus, SLOT(fetchAtEnd(bool,Common::DecodedClusters,Common::RejectedClusters,Common::ImageInfo)) );

    mImage.load("shared_resources/1.jpg");
    if (mImage.isNull())
        qDebug()<<"Failed to load image:";

    mCurrentCluster = 0;
    mTotalClusters = 30;

    int nblocks = mImage.width()*mImage.height()/8/8;
    for (int i=0, c=0, previ=0; i<nblocks; ++c) {
        i += nblocks/mTotalClusters;
        DecodedClusterInfo dci = { c, previ, qMin(nblocks-1, i) };
        mDC.push_back( dci );
        previ = i + 1;
    }
    qDebug()<<mDC.size();

    connect(&mProgressTimer, SIGNAL(timeout()), this, SLOT(onDecodeCluster()) );
    mProgressTimer.setInterval(100);
}

void Decoder::start(int clusterNo, const QString &shmemPath, const QString &wspacePath)
{
    qDebug()<<__FUNCTION__<<clusterNo<<shmemPath<<wspacePath;

    QDir().mkpath(wspacePath);    
    mImagePath = wspacePath + "/image.png";
    
    
    
    startDecoding();
}

void Decoder::exit()
{
    qApp->quit();
}

void Decoder::baseline(int cluster)
{
    qDebug()<<__FUNCTION__<<cluster;

    startDecoding();
}

void Decoder::onDecodeCluster()
{
    if (mCurrentCluster >= mDC.size()) {
        emit decodingEnd(false);
        stopDecoding();
    }

    emit progress(mCurrentCluster, mDC[mCurrentCluster].blockBegin, mDC.back().blockEnd +1);
    if ((++mCurrentCluster %10) == 0) {

        stopDecoding();
    }
}

void Decoder::startDecoding()
{
    mProgressTimer.start();
}

void Decoder::stopDecoding()
{
    
    emit fetchAtEnd(false, mDC, RejectedClusters(), Jpeg::storeImage(mImage, mImagePath));
    mProgressTimer.stop();
}

