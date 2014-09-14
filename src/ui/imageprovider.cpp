#include <ui/imageprovider.h>
#include <QtDebug>

namespace Ui {

ImageProviderAdaptor::ImageProviderAdaptor()
: mOwner(nullptr)
{
}

ImageProviderAdaptor::~ImageProviderAdaptor()
{
    if (mOwner) {
        mOwner->removeAdaptor(mName);
    }
}

///

ImageProvider::ImageProvider()
: QQuickImageProvider(Image)
{
}

ImageProvider::~ImageProvider()
{
    for (auto a: mAdaptors) 
        a->disowned();
    
}

void ImageProvider::addAdaptor(const QString &name, ImageProviderAdaptor *adaptor)
{
    if (mAdaptors.contains(name)) {
        qDebug()<<"Ui::ImageProvider: failed to addAdaptor (Already added). "<<name;
        return;
    }
    mAdaptors.insert(name, adaptor);
    adaptor->owned(name, this);
}

void ImageProvider::removeAdaptor(const QString &name)
{
    auto itr = mAdaptors.find(name);
    if (itr == mAdaptors.end()) {
        qDebug()<<"Ui::ImageProvider: failed to removeAdaptor (Not found). "<<name;
        return;
    }
    
    (*itr)->disowned();
    mAdaptors.erase(itr);
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestSize)
{
    int sepIdx = id.indexOf("/");
    if (sepIdx == -1) {
        qDebug()<<"Ui::ImageProvider::requestImage(). Failed to parse id. "<<id;
        return QImage();
    }
    
    QString adpName = id.left(sepIdx);
    QString imgId = id.right(id.size() -sepIdx -1);
    auto itr = mAdaptors.find( adpName );
    if (itr == mAdaptors.end()) {
        qDebug()<<"Ui::ImageProvider::requestImage(). Adaptor no found. "<<adpName;
        return QImage();
    }
    
    QImage img = (*itr)->get(imgId);
    *size = img.size();
    return img;
}

}
