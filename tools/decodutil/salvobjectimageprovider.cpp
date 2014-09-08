#include "salvobjectimageprovider.h"
#include <ui/salvjpegobject.h>
#include <QtDebug>

SalvObjectImageProvider::SalvObjectImageProvider(SalvJpegObject *object)
: QQuickImageProvider( QQuickImageProvider::Image )
, mSalv(object)
{
}

QImage SalvObjectImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QStringList params = id.split("/", QString::KeepEmptyParts);
    
    if (params.size() == 2 && params[0] == "image") {
        QImage res = mSalv->image();
        *size = res.size();
        return res;
        
    } else if (params.size() == 3 && params[0] == "shade") {
        bool ok;
        int blockEnd = params[2].toInt(&ok);
        if (ok) {
            QImage res = mSalv->shade(blockEnd);
            *size = res.size();
            return res;
        }    
    } 
    
    qDebug()<<"ImageProvider: Failed to parse: "<<id;
        
    return QImage();
}