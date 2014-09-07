#include "loggingcheck.h"
#include <jpeg/picojpegdecodr.h>

LoggingCheck::LoggingCheck(QObject *parent) :
    Check(parent)
{
}

const Check::FrameDescription &LoggingCheck::chooseBaseline(const Check::FrameDescription_v &frames)
{
    for (const Check::FrameDescription &d: frames) {
        //fill res
        if (d.clustersBegin == d.clustersEnd || d.frame==nullptr || d.frame->type() != Jpeg::PicoJpegDecodFrame::JpegContextType )
            continue;
            
        Jpeg::PicoJpegDecodFrame *frame = static_cast<Jpeg::PicoJpegDecodFrame *>(d.frame);
        DecodedClusterInfo clusterInfo = { *d.clustersBegin, 0, frame->cursor.currentBlockIndex() };
        mRes.push_back( clusterInfo );
    }

    return frames.back();
}
