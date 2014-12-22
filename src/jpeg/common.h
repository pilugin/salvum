#ifndef COMMON_H
#define COMMON_H

#include <QImage>
#include <QtCore/QByteArray>
#include <picojpeg/picojpeg.h>
#include <jpeg/imagecursor.h>

namespace Jpeg {

struct DecodrState
{
    bool lastWasFF; //< used to find FFD9 splitted in 2 clusters. This param is set inside fetchCallback
    QByteArray buffer;
    int bufferPos;
    int bytesRead; //< used to check FFD9,

    QByteArray pjpegCtxt;
    pjpeg_image_info_t imgInfo;
    ImageCursor cursor;

    //
    bool decodOk;
    bool checkOk;
    bool end;
    struct {
        int blockBegin;
        QList<int> pixels;
    } savedPixels;

    void savePixels(const DecodrState &prevFrame);

    bool operator==(const DecodrState &other) const;
    bool operator!=(const DecodrState &other) const     { return !(*this == other); }

//private:
//    friend class PicoJpegDecodr;
    DecodrState(QImage *image =nullptr);
};

}

#endif // COMMON_H