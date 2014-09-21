#include "loggingcheck.h"
#include <jpeg/picojpegdecodr.h>
#include <QtDebug>

using namespace Core;
using namespace Common;

LoggingCheck::LoggingCheck(QObject *parent) :
    Check(parent)
{
}

Check::FrameDescription_itr LoggingCheck::chooseBaseline(const Check::FrameDescription_v &frames)
{
    qDebug("CHECK CHOOSE_BASELINE");

    for (const Check::FrameDescription &d: frames) {
        //fill res
        if (d.clustersCount==0 || d.frame==nullptr || d.frame->type() != Jpeg::PicoJpegDecodFrame::JpegContextType )
            continue;
            
        Jpeg::PicoJpegDecodFrame *frame = static_cast<Jpeg::PicoJpegDecodFrame *>(d.frame);
        DecodedClusterInfo clusterInfo = {
            clusters()[d.clustersPos],
            mRes.size() > 0 ? mRes.back().blockEnd+1 : 0,
            frame->cursor.currentBlockIndex() };
        mRes.push_back( clusterInfo );
    }

    return frames.begin() + frames.size() - 1;
}
