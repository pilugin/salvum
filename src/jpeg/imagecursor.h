#ifndef IMAGECURSOR_H
#define IMAGECURSOR_H

#include <QImage>
#include <QPair>

class ImageCursor
{
public:
    ImageCursor();
    ImageCursor(const ImageCursor &other);
    ImageCursor &operator=(const ImageCursor &other);

    void setCanvas(QImage *canvas);
    bool initCanvas(const QSize &size, QImage::Format format =QImage::Format_RGB888);

    void addBlock(const uchar *r, const uchar *g, const uchar *b);

    void restart() { mBlockX = mBlockY = 0; }
    bool atEnd() const;

    int currentBlockIndex() const;
private:
    bool isBlockValid() const;
    void incBlock();

    QImage *mCanvas;
    int mBlockX;
    int mBlockY;
};

#endif // IMAGEADAPTOR_H
