#include <ui/bitmapinfomodel.h>

BitmapInfoModel::BitmapInfoModel(QObject *parent) : QObject(parent)
{
}

void BitmapInfoModel::setInfo(const BitmapInfo &info)
{
    mInfo = info;
    emit updated();
}