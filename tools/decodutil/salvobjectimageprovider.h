#ifndef SALVJPEGIMAGEPROVIDER_H
#define SALVJPEGIMAGEPROVIDER_H

#if QT_VERSION >= 0x050000
#	include <QQuickImageProvider>
#else
#	include <QDeclarativeImageProvider>
#	define QQuickImageProvider QDeclarativeImageProvider
#endif

namespace Ui {
class SalvJpegObject;
}

class SalvObjectImageProvider : public QQuickImageProvider
{
public:
    SalvObjectImageProvider(Ui::SalvJpegObject *object);
    
    QImage requestImage(const QString &id, QSize *size, const QSize &requestSize);
private:
    Ui::SalvJpegObject *mSalv;    
};

#endif