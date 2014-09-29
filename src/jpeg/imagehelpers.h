#ifndef IMAGEHELPERS_H
#define IMAGEHELPERS_H

#include <common/types.h>
#include <QImage>
#include <QPair>

namespace Jpeg {

QImage image(const Common::Pixmap &pixmap);
QImage highlight(const QImage &base, int blockEnd);

QPair<QRect, QRect> shade(int width, int height, int blockEnd);

inline QPair<QRect, QRect> shade(const QImage &base, int blockEnd) { return shade(base.width(), base.height(), blockEnd); }

inline QPair<QRect, QRect> shade(const Common::ImageInfo &imageInfo, int blockEnd) { return shade(imageInfo.width, imageInfo.height, blockEnd); }

Common::Pixmap dbusPixmap(const QImage &image);

Common::ImageInfo storeImage(const QImage &image, const QString &path);

QPair<QImage, int> imageFragment(int width, int height, const Common::RejectedClusterInfo &rc); //< ret Image + vertical offset

} // eons Jpeg

#endif