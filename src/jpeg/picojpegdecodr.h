#ifndef PICOJPEGDECODR_H
#define PICOJPEGDECODR_H

#include "if/idecod.h"
#include "if/ilog.h"
#include "picojpeg/picojpeg.h"
#include "jpeg/imagecursor.h"
#include "util/singleton.h"

#include <QImage>

class ICheck;

struct PicoJpegDecodContext
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


class PicoJpegDecodr : public IDecod, public Singleton<PicoJpegDecodr>
{
public:
    PicoJpegDecodr();
    ~PicoJpegDecodr();

    bool restart(IFetch *fetch);
    bool decodeCluster();
    void revert();

    bool done() const;
    QVector<int> usedClusters() const   { return mUsedClusters; }

    int latestBlock() const;
    QImage &image();

private:

    IFetch *mFetch;
    ICheck *mCheck;
    QImage mImage;

    PicoJpegDecodContext mContext;
    PicoJpegDecodContext save() const;
    void restore(const PicoJpegDecodContext &ctxt);
    PicoJpegDecodContext mBackupContext;

    bool checkFFD9() const;

    int mBlockCount;

    bool mWasFetched;
    QVector<int> mUsedClusters;
    void clearUsedClusters();

    bool mDone;


    static unsigned char fetchCallback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *param);
    unsigned char fetchCallback(unsigned char *pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read);
};

#endif // PICOJPEGDECODR_H
