#include "imagecursor.h"
#include <QtDebug>

namespace Jpeg {

ImageCursor::ImageCursor()
    : mCanvas(0)
{
}

ImageCursor::ImageCursor(const ImageCursor &other)
{
    operator =(other);
}

ImageCursor &ImageCursor::operator =(const ImageCursor &other)
{
    mBlockX = other.mBlockX;
    mBlockY = other.mBlockY;
    mCanvas = other.mCanvas;
    return *this;
}

void ImageCursor::setCanvas(QImage *canvas)
{
    mCanvas = canvas;
}

bool ImageCursor::initCanvas(const QSize &size, QImage::Format format)
{
    mBlockX = mBlockY = 0;
    *mCanvas = QImage(size, format);

    return !mCanvas->isNull();
}

void ImageCursor::addBlock(const uchar *r, const uchar *g, const uchar *b)
{
    if (!isBlockValid())
        return;

    int baseX = mBlockX * 8;
    int baseY = mBlockY * 8;

    for (int y=0; y<8; ++y)
        for (int x=0; x<8; ++x)
            mCanvas->setPixel(baseX + x, baseY + y, qRgb(*r++, *g++, *b++));

    incBlock();
}

bool ImageCursor::atEnd() const
{
    return (mBlockX == 0) && (mBlockY == mCanvas->height()/8);
}

int ImageCursor::currentBlockIndex() const
{
    return (mBlockY * mCanvas->width() / 8) + mBlockX;
}

bool ImageCursor::isBlockValid() const
{
    return      (mBlockX < (mCanvas->width()/8)) && (mBlockX >= 0)
            &&  (mBlockY < (mCanvas->height()/8)) && (mBlockY >= 0);
}

void ImageCursor::incBlock()
{
    if (++mBlockX >= (mCanvas->width()/8)) {
        mBlockX = 0;
        ++mBlockY;
    }
}

} // eons Jpeg
