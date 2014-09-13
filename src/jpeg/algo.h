#ifndef JPEG_ALGO_H
#define JPEG_ALGO_H

#include <common/types.h>

namespace Jpeg {

Common::BitmapInfo processBitmap(const QByteArray &bm, QList<int> &jpegHeads, QList<int> &goodHeads);

}

#endif