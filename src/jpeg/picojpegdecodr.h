#ifndef PICOJPEGDECODR_H
#define PICOJPEGDECODR_H

#include <core/decodr.h>
#include <util/ilog.h>
#include <picojpeg/picojpeg.h>
#include <jpeg/imagecursor.h>
#include <util/singleton.h>

#include <QImage>
#include <QStack>

namespace Jpeg {

class ICheck;

// This struct incapsulates the whole decoding context,
// it is used for save/restore functionality.
// Note: FFD9 = End-Of-Image marker 
class PicoJpegDecodFrame : public DecodrFrame
{
public:
    enum { JpegContextType = DecodrFrame::CustomType +42 };

    PicoJpegDecodFrame(QImage *image =nullptr);

    bool lastWasFF; //< used to find FFD9 splitted in 2 clusters. This param is set inside fetchCallback
    QByteArray buffer;
    int bufferPos;
    int bytesRead; //< used to check FFD9,

    QByteArray pjpegCtxt;
    pjpeg_image_info_t imgInfo;
    ImageCursor cursor;
    
    PicoJpegDecodFrame *clone() const;
private:
    static int id_gen;    
};

// Note: Signleton pattern is used to guarantee that only one instance is created.
// Such limitation comes with picojpeg library, as it uses lots of global variables :(
class PicoJpegDecodr : public Decodr, public Singleton<PicoJpegDecodr>
{
public:
    PicoJpegDecodr(ICheck *check, QObject *parent =nullptr);

    bool restart(Fetch *fetch);
    void resume();
    void loadFrame(const DecodrFrame &frame);
    

    bool isDone() const;
    
private:
    bool decodeCluster();
    void saveFrame(PicoJpegDecodFrame &outFrame);
    
    bool checkFFD9() const;
    int latestBlock() const;
    
    Fetch *mFetch;
    ICheck *mCheck;
    QImage mImage;
    PicoJpegDecodFrame mFrame;
    
    int mBlockCount;
    bool mWasFetched;
    bool mDone;

    static unsigned char fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param);
    unsigned char fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read);
};

} // eons Jpeg

#endif // PICOJPEGDECODR_H
