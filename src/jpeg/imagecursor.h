#ifndef IMAGECURSOR_H
#define IMAGECURSOR_H

#include <QImage>
#include <QColor>
#include <QPair>
#include <functional>

namespace Jpeg {

class ImageCursor
{
public:
    ImageCursor();
    ImageCursor(const ImageCursor &other);
    ImageCursor &operator=(const ImageCursor &other);

    void setCanvas(QImage *canvas);
    bool initCanvas(const QSize &size, QImage::Format format =QImage::Format_RGB888);

    void addBlock(const uchar *r, const uchar *g, const uchar *b);
    void addBlock(uint color);
    void addBlock(QList<int>::const_iterator &itr);

    void restart() { mBlockX = mBlockY = 0; }
    bool atEnd() const;

    void setCurrentBlockIndex(int block);
    int currentBlockIndex() const;
    QImage *canvas() const { return mCanvas; }
private:

    void addBlock_internal(std::function<uint ()> func);

    bool isBlockValid() const;
    void incBlock();

    QImage *mCanvas;
    int mBlockX;
    int mBlockY;
};

} // eons Jpeg

#endif // IMAGEADAPTOR_H
