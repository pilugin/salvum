#ifndef IMAGEHELPERS_H
#define IMAGEHELPERS_H

#include <dbus/dbustypes.h>
#include <QImage>

namespace Jpeg {

QImage image(const Pixmap &pixmap);
QImage highlight(const QImage &base, int blockEnd);

Pixmap dbusPixmap(const QImage &image);

} // eons Jpeg

#endif