#ifndef IMAGEHELPERS_H
#define IMAGEHELPERS_H

#include <common/types.h>
#include <QImage>
#include <QPair>

namespace Jpeg {

QImage image(const Common::Pixmap &pixmap);
QImage highlight(const QImage &base, int blockEnd);

QPair<QRect, QRect> shade(const QImage &base, int blockEnd);

Common::Pixmap dbusPixmap(const QImage &image);

} // eons Jpeg

#endif