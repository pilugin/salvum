#ifndef SALVJPEGOBJECT_H
#define SALVJPEGOBJECT_H

#include <dbus/declareDBusMetatypes.h>
#include <QtCore/QObject>
#include <QImage>

namespace Ui {

class DecodedClustersModel;

class SalvJpegObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id                       READ id                 NOTIFY idChanged)
    Q_PROPERTY(bool inProgress              READ inProgress         NOTIFY inProgressChanged)
    Q_PROPERTY(bool complete                READ complete           NOTIFY completeChanged)
    Q_PROPERTY(QString image                READ imageId            NOTIFY imageChanged)    
    Q_PROPERTY(QString shade                READ shadeId            NOTIFY shadeChanged)
//    Q_PROPERTY(int imageWidth               READ imageWidth         NOTIFY imageWidthChanged)
//    Q_PROPERTY(int imageHeight              READ imageHeight        NOTIFY imageHeight)
    Q_PROPERTY(QObject *decodedClusters     READ decodedClusters    NOTIFY decodedClustersChanged)
public:
    SalvJpegObject(int id_, const QString &imageProviderPrefix, QObject *parent =nullptr);
    
    int id() const          { return mId; }
    bool inProgress() const { return mInProgress; }
    bool complete() const   { return mComplete; }
    QString imageId() const;
    QString shadeId() const;
    QObject *decodedClusters() const;
    
    QImage image() const { return mImage; } 
    QImage shade(int blockEnd) const;
    
signals:
    void idChanged(int); //< never emitted, as id = const
    void inProgressChanged(bool);
    void completeChanged(bool);
    void imageChanged(QString);
    void shadeChanged(QString);
    void decodedClustersChanged(QObject *);

    void baseline(int clusterNo);

public slots:    
    void decodrInProgress();
    void decodrAtEnd(bool complete, const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);
    void decodrAtEnd(bool complete, const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const QImage &image);
    void currentClusterChanged(int clusterNo, int blockBegin, int blockEnd);
    void baselineSelected(int clusterNo);

private:
protected:
    const int mId;
    const QString mImageProviderPrefix;
    const QString mImageId;
    
    QString mShadeId;
        
    bool mInProgress;
    bool mComplete;
    Common::DecodedClusters mDecodedClusters;
    QImage mImage;
    mutable struct {
        QImage image;
        int blockEnd;
    } mShade;
    
    DecodedClustersModel *mSubmodel;
};

}

#endif
