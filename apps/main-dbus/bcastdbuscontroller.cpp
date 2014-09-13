#include "bcastdbuscontroller.h"

#include <QtDBus>
#include <QtDebug>

using namespace Common;

BcastDbusController::BcastDbusController(QObject *parent)
: QObject(parent)
{
    mBcast = new org::salvum::Broadcast("org.salvum.Broadcast", "/broadcast", 
                                                            QDBusConnection::sessionBus(), this);
    connect(mBcast, SIGNAL(bitmapProcessed(QList<int>,QList<int>,Common::BitmapInfo)), 
            this, SLOT(onBitmapProcessed(QList<int>,QList<int>,Common::BitmapInfo)));
}

void BcastDbusController::onBitmapProcessed(const QList<int> &jpegHeads, const QList<int> &goodHeads, Common::BitmapInfo info)
{
    qDebug()<<"BitmapInfo:";
    qDebug("\t%08d - jpegHeads", info.jpegHeads);
    qDebug("\t%08d - goodHeads", info.goodHeads);
    qDebug("\t%08d - zeros", info.zeros);
    qDebug("\t%08d - goods", info.goods);
    qDebug("\t%08d - decodable", info.decodable);
    qDebug("\t%08d - starts", info.starts);

    qDebug("\t\t#jpegHeads=%d; #goodHeads=%d", jpegHeads.size(), goodHeads.size() );

    emit bitmapInfoUpdated(info);
    emit jpegHeadsUpdated(jpegHeads);
    emit goodHeadsUpdated(goodHeads);
}

void BcastDbusController::setSource(const QString &mediaPath, const QString &bitmapPath)
{
    mBcast->setSource(mediaPath, bitmapPath);
}

