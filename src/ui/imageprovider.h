#ifndef UI_IMAGE_PROVIDER_H
#define UI_IMAGE_PROVIDER_H

#if QT_VERSION >= 0x050000
#   include <QQuickImageProvider>
#else
#   include <QDeclarativeImageProvider>
#   define QQuickImageProvider QDeclarativeImageProvider
#endif

#include <QMap>

namespace Ui {

class ImageProvider;

class ImageProviderAdaptor
{
    friend class ImageProvider;
public:
    ImageProviderAdaptor();
    virtual ~ImageProviderAdaptor();
    
    virtual QImage get(const QString &image) const =0;
    QString name() const { return mName; }
    QString imagePrefix() const;
private:
    void owned(const QString &provName, const QString &name, ImageProvider *owner);
    void disowned();

    QString mName;
    QString mProvName;
    ImageProvider *mOwner;            
};

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    ~ImageProvider();
    
    QString name() const { return mName; }
    void setName(const QString &name) { mName=name; }
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    
    void addAdaptor(const QString &name, ImageProviderAdaptor *adaptor);
    void removeAdaptor(const QString &name);
private:
    QMap<QString, ImageProviderAdaptor *> mAdaptors;
    QString mName;
};

}

#endif // UI_IMAGE_PROVIDER_H
