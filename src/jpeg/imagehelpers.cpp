#include "imagehelpers.h"

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

QImage highlight(const QImage &base, int blockEnd)
{
    QImage res( base.width(), base.height(), QImage::Format_ARGB32 );
    res.fill(qRgba(0,0,0,0));
    
    QRgb shadePixel = qRgba(0xff,0xff,0xff,0x80);
    int shadedBlock = blockEnd +1;
    int blockCount = (res.width() * res.height() / 64);
    
    for (; shadedBlock<blockCount; ++shadedBlock) {
    
	int blockX = shadedBlock % (res.width() / 8);
	int blockY = shadedBlock / (res.width() / 8);
	int baseX = blockX * 8;
	int baseY = blockY * 8;
    
	for (int x=0; x<8; ++x)
	    for (int y=0; y<8; ++y)
		res.setPixel(baseX +x, baseY +y, shadePixel);
    }
    
    return res;
}

} // eons Jpeg
