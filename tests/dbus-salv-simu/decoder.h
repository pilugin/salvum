#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QImage>
#include "org.salvum.DecodrCtrl.h"

class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(org::salvum::DecodrCtrl *dbus, QObject *parent = 0);

signals:
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);
    void fetchAtEnd(bool complete, Common::DecodedClusters decodedClusters, Common::RejectedClusters rejectedClusters, Common::ImageInfo imageInfo);
    void thumbnailCreated(const QString &path);
    void decodingEnd(bool success);

public slots:
    void start(int clusterNo, const QString &shmemPath, const QString &wspacePath);
    void exit();
    void baseline(int cluster);

    void onDecodeCluster();
private:
    org::salvum::DecodrCtrl *mDecodrCtrl;

    void startDecoding();
    void stopDecoding();

    QTimer mProgressTimer;

    QImage mImage;
    QString mImagePath;
    Common::DecodedClusters mDC;
    Common::RejectedClusters mRC;

    int mCurrentCluster;
    int mTotalClusters;
};

#endif // DECODER_H
