#include <ui/bitmapinfomodel.h>

using namespace Common;

namespace Ui {

BitmapInfoModel::BitmapInfoModel(QObject *parent) : QObject(parent)
{
}

void BitmapInfoModel::setInfo(const BitmapInfo &info)
{
    mInfo = info;
    emit updated();
}

}
