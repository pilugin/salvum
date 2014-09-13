#include "salvjpegimageprovider.h"
#include "salvjpegmodel.h"
#include <ui/salvjpegobject.h>
#include <QtDebug>

using namespace Ui;

SalvJpegImageProvider::SalvJpegImageProvider(SalvJpegModel *model)
: QQuickImageProvider( QQuickImageProvider::Image )
, mModel(model)
{
}

QImage SalvJpegImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QStringList params = id.split("/", QString::KeepEmptyParts);
    bool parseOk = false;
    int jpegId = 0;
    
    if (params.size() == 2 && params[0] == "image") {
        bool ok;
        jpegId = params[1].toInt(&ok);
        if (ok) {
            parseOk = true;
            if (SalvJpegObject *o = mModel->getSalvJpeg(jpegId)) {
                QImage res = o->image();
                *size = res.size();
                return res;
            }
        }
        
    } else if (params.size() == 3 && params[0] == "shade") {
        bool ok;
        jpegId = params[1].toInt(&ok);
        if (ok) {
            int blockEnd = params[2].toInt(&ok);
            if (ok) {
                parseOk = true;
                if (SalvJpegObject *o = mModel->getSalvJpeg(jpegId)) {
                    QImage res = o->shade(blockEnd);
                    *size = res.size();
                    return res;
                }
            }        
        }    
    } 
    
    if (parseOk)
        qDebug()<<"ImageProvider: id not found:" << jpegId << "; " << id;
    else
        qDebug()<<"ImageProvider: Failed to parse: "<<id;
        
    return QImage();
}