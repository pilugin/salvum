#include <jpeg/imagehelpers.h>
#include <jpeg/imagecursor.h>
#include <QtDebug>

using namespace Common;

namespace Jpeg {

QImage image(const Pixmap &pixmap)
{
    QImage res( pixmap.width, pixmap.height, QImage::Format_RGB888);
    
    int i=0;
    for (int y=0; y<res.height(); ++y)
        for (int x=0; x<res.width(); ++x)
            res.setPixel(x, y, pixmap.pixels[i++]);
    
    return res;
}

Pixmap dbusPixmap(const QImage &image)
{
    Pixmap res;
    res.width = image.width();
    res.height = image.height();
    for (int y=0; y<image.height(); ++y)
        for (int x=0; x<image.width(); ++x)
            res.pixels.push_back(image.pixel(x, y));
    
    return res;
}

ImageInfo storeImage(const QImage &image, const QString &path)
{
    ImageInfo ii;
    ii.width = image.width();
    ii.height = image.height();
    if (image.save(path, "PNG"))
        ii.imagePath = path;
    return ii;
}

QImage highlight(const QImage &base, int blockEnd)
{
    QImage res( base.width(), base.height(), QImage::Format_ARGB32 );
    res.fill(qRgba(0,0,0,0));
    
    QVector<QRgb> shadePixel = { qRgba(0x00,0x00,0x00,0x80), qRgba(0x80, 0x80, 0x80, 0x80) };
    int shadedBlock = blockEnd +1;
    int blockCount = (res.width() * res.height() / 64);
    
    for (; shadedBlock<blockCount; ++shadedBlock) {
        int blockX = shadedBlock % (res.width() / 8);
        int blockY = shadedBlock / (res.width() / 8);
        int baseX = blockX * 8;
        int baseY = blockY * 8;
            
        for (int y=0; y<8; ++y) {
            if ((baseY +y) >= res.height()) {
                break;
            }
            for (int x=0; x<8; ++x)
                res.setPixel(baseX +x, baseY +y, shadePixel[ (blockX + blockY)%shadePixel.size() ]);
        }
    }
    
    return res;
}

QPair<QRect, QRect> shade(int width, int height, int blockEnd)
{
    int blockX = (blockEnd +1) % (width / 8);
    int blockY = (blockEnd +1) / (width / 8);
    
    QPair<QRect, QRect> rects;
    
    rects.first.setLeft( blockX *8 );
    rects.first.setTop( blockY *8 );
    rects.first.setRight( width-1 );
    rects.first.setBottom( height-1 );
    
    rects.second.setLeft(0);
    rects.second.setTop( (blockY +1) *8 );
    rects.second.setRight( rects.first.left() );
    rects.second.setBottom( height-1 );
    
    return rects;
}

QPair<QImage, int> imageFragment(int width, int height, const Common::RejectedClusterInfo &rc)
{
    QImage res;
    
    int w = width;
    int blockBeginX = rc.blockBegin % (width/8);
    int h = (  (blockBeginX + (rc.pixels.size()/8/8)) + (width/8) -1) / (width/8); //< int division w/ceiling; ceil(x/y) == (x+y-1)/y
    h *= 8;
    
    qDebug()<<"IMAGE_FRAGMENT!!"<<width<<height<<rc.pixels.size();
    qDebug()<<"\t\t" << w<<h<<blockBeginX;
    
    ImageCursor cursor;
    cursor.setCanvas( &res );
    cursor.initCanvas(QSize(w,h), QImage::Format_ARGB32_Premultiplied);
    for (int i=0; i<blockBeginX; ++i)
        cursor.addBlock(qRgba(0,0,0,0));
    
    for (auto itr=rc.pixels.begin(); itr!=rc.pixels.end(); )
        cursor.addBlock( itr );
        
    while (!cursor.atEnd())
        cursor.addBlock(qRgba(0,0,0,0));

    return qMakePair(res, (rc.blockBegin/(width/8))*8);
}

} // eons Jpeg
