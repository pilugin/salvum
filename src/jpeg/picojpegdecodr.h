#ifndef PICOJPEGDECODR_H
#define PICOJPEGDECODR_H

#include "if/idecod.h"
#include "if/ilog.h"
#include "picojpeg/picojpeg.h"
#include "jpeg/imagecursor.h"
#include "util/singleton.h"

#include <QImage>

namespace Jpeg {

class ICheck;

// This struct incapsulates the whole decoding context,
// it is used for save/restore functionality.
// Note: FFD9 = End-Of-Image marker 
struct PicoJpegDecodContext : public IDecod::Context
{
    PicoJpegDecodContext();

    bool lastWasFF; //< used to find FFD9 splitted in 2 clusters. This param is set inside fetchCallback
    QByteArray buffer;
    int bufferPos;
    int bytesRead; //< used to check FFD9,

    QByteArray pjpegCtxt;
    pjpeg_image_info_t imgInfo;
    ImageCursor cursor;
};

// Note: Signleton pattern is used to guarantee that only one instance is created.
// Such limitation comes with picojpeg library, as it uses lots of global variables :(
class PicoJpegDecodr : public IDecod, public Singleton<PicoJpegDecodr>
{
public:
    PicoJpegDecodr(ICheck *check);
    ~PicoJpegDecodr();

    bool restart(IFetch *fetch);
    bool decodeCluster();
    void revert(int steps);

    bool done() const;
    
    int historyLength() const { return mHistory.size(); }
    PicoJpegDecodContext *historyFrame(int frameNo) const;

private:
    int latestBlock() const;

    IFetch *mFetch;
    ICheck *mCheck;
    QImage mImage;

    PicoJpegDecodContext &context() 	{ return mHistory.top(); }
    QStack<PicoJpegDecodContext> 	mHistory;
    
    void saveContext();
    
    bool checkFFD9() const;

    int mBlockCount;
    bool mWasFetched;
    
    bool mDone;


    static unsigned char fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param);
    unsigned char fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read);
};

} // eons Jpeg

#endif // PICOJPEGDECODR_H
