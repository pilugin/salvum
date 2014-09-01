#ifndef SALVJPEGOBJECT_H
#define SALVJPEGOBJECT_H

#include "if/dbus/declareDBusMetatypes.h"
#include <QtCore/QObject>
#include <QImage>

class DecodedClustersModel;

class SalvJpegObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id 		READ id 	NOTIFY idChanged)
    Q_PROPERTY(bool inProgress 	READ inProgress NOTIFY inProgressChanged)
    Q_PROPERTY(bool complete 	READ complete	NOTIFY completeChanged)
    Q_PROPERTY(QString image	READ imageId	NOTIFY imageChanged)
    Q_PROPERTY(QString shade	READ shadeId	NOTIFY shadeChanged)
    Q_PROPERTY(QObject *decodedClusters	READ decodedClusters	NOTIFY decodedClustersChanged)
public:
    SalvJpegObject(int id_, const QString &imageProviderPrefix, QObject *parent =nullptr);
    
    int id() const		{ return mId; }
    bool inProgress() const	{ return mInProgress; }
    bool complete() const	{ return mComplete; }
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
    void decodrAtEnd(bool complete, const DecodedClusters &decodedClusters, const Pixmap &pixmap);
    void currentClusterChanged(int clusterNo, int blockBegin, int blockEnd);
    void baselineSelected(int clusterNo);

private:
    const int mId;
    const QString mImageProviderPrefix;
    const QString mImageId;
    
    QString mShadeId;
        
    bool mInProgress;
    bool mComplete;
    DecodedClusters mDecodedClusters;
    QImage mImage;
    mutable struct {
        QImage image;
        int blockEnd;
    } mShade;
    
    DecodedClustersModel *mSubmodel;
};

#endif
