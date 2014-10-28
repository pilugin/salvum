#include <jpeg/imagecursor.h>
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
    addBlock_internal( 
        [&]() { return qRgb(*r++, *g++, *b++); }
        );
}

void ImageCursor::addBlock(uint color)
{
    addBlock_internal( 
        [=]() { return color; } 
        );
}

void ImageCursor::addBlock(QList<int>::const_iterator &itr)
{
    addBlock_internal( 
        [&]() { return static_cast<uint>(*itr++); }
        );
}

void ImageCursor::addBlock_internal(std::function<uint ()>func)
{
    if (!isBlockValid())
        return;
    int baseX = mBlockX * 8;
    int baseY = mBlockY * 8;
    for (int y=0; y<8; ++y)
        for (int x=0; x<8; ++x)
            mCanvas->setPixel(baseX + x, baseY + y, func());
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

void ImageCursor::setCurrentBlockIndex(int block)
{
    mBlockY = block / (mCanvas->width() / 8);
    mBlockX = block % (mCanvas->width() / 8);
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
    
    if (isBlockValid()) 
        for (int i=0; i<8; ++i) {
            mCanvas->setPixel(mBlockX * 8 +7-i, mBlockY * 8 +i, qRgb(0xFF,0,0));
            mCanvas->setPixel(mBlockX * 8 +i, mBlockY * 8 +i, qRgb(0xFF,0,0));
            mCanvas->setPixel(mBlockX * 8, mBlockY * 8 +i, qRgb(0xFF,0,0));
        }
}

int ImageCursor::totalBlocks() const
{
    return mCanvas->width()*mCanvas->height()/8/8;
}

} // eons Jpeg
