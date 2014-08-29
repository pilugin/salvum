#ifndef SALVJPEGIMAGEPROVIDER_H
#define SALVJPEGIMAGEPROVIDER_H

#if QT_VERSION >= 0x050000
#	include <QQuickImageProvider>
#else
#	include <QDeclarativeImageProvider>
#	define QQuickImageProvider QDeclarativeImageProvider
#endif

class SalvJpegModel;

class SalvJpegImageProvider : public QQuickImageProvider
{
public:
    SalvJpegImageProvider(SalvJpegModel *model);
    
    QImage requestImage(const QString &id, QSize *size, const QSize &requestSize);
private:
    SalvJpegModel *mModel;    
};

#endif