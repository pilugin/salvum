#include "salvdbuscheck.h"
#include <jpeg/imagehelpers.h>
#include <jpeg/picojpegdecodr.h>

#include <QtCore/QEventLoop>
#include <QtDebug>

class SalvDbusCheck::Private
{
public:
    explicit Private(QObject *parent)
    : eventLoop(new QEventLoop(parent))
    {
    }
    
    static Common::DecodedClusters decodedClusters(const Core::Check::FrameDescription_v &frames, const QVector<int> &clusters)
    {
        Common::DecodedClusters res;
        for (const Core::Check::FrameDescription &fd: frames) {
            Q_ASSERT(fd.frame->type() == Jpeg::PicoJpegDecodFrame::JpegContextType);
            if (!fd.accepted)
                continue;
        
            Common::DecodedClusterInfo i;
            i.blockBegin = res.size()>0 ? res.back().blockEnd +1 : 0;
            i.blockEnd = static_cast<Jpeg::PicoJpegDecodFrame *>(fd.frame)->cursor.currentBlockIndex() -1;
            
            for (int c=0; c<fd.clustersCount; ++c) {
                i.clusterNo = clusters[ fd.clustersPos +c ];
                res.push_back( i );
            }
        }
        return res;
    }
    
    static Common::RejectedClusters rejectedClusters(const Core::Check::FrameDescription_v &frames, const QVector<int> &clusters)
    {
        Common::RejectedClusters res;
        for (const Core::Check::FrameDescription &fd: frames) {
            Q_ASSERT(fd.frame->type() == Jpeg::PicoJpegDecodFrame::JpegContextType);
            if (fd.accepted)
                continue;
        
            auto *f = static_cast<Jpeg::PicoJpegDecodFrame *>(fd.frame);
            Common::RejectedClusterInfo i;
            i.blockBegin = f->savedPixels.blockBegin;
            i.pixels = f->savedPixels.pixels;
            
            for (int c=0; c<fd.clustersCount; ++c) {
                i.clusterNo = clusters[ fd.clustersPos +c ];
                res.push_back( i );
            }
        }
        return res;
    }
    
    
    
    
    
    QEventLoop *eventLoop;    
    int baselineClusterNo;
};

////////////////////////////

SalvDbusCheck::SalvDbusCheck(QObject *parent)
: Core::Check(parent)
, m_d(new Private(this))
{
}

SalvDbusCheck::~SalvDbusCheck()
{
    delete m_d;    
}
    
void SalvDbusCheck::baseline(int clusterNo)
{
    m_d->baselineClusterNo = clusterNo;
    breakEventLoop(0);
}
    
SalvDbusCheck::FrameDescription_itr SalvDbusCheck::chooseBaseline(const SalvDbusCheck::FrameDescription_v &frames)
{
    // sending atEnd by DBus
    emit atEnd(false, 
            Private::decodedClusters(frames, clusters()),
            Private::rejectedClusters(frames, clusters()),
            Jpeg::dbusPixmap( *static_cast<Jpeg::PicoJpegDecodFrame *>(frames.back().frame)->cursor.canvas() ) 
            );
            
    // wait baseline or interrupt
    m_d->baselineClusterNo = -1;            
    qDebug("~~~STARTING NESTED EVENTLOOP");            
    int rc = m_d->eventLoop->processEvents();
    qDebug("~~~Exit NESTED EVENTLOOP: retcod=%d; baselineCluster=%08X", rc, m_d->baselineClusterNo);
    
    // check baseline; exit if -1
    if (m_d->baselineClusterNo != -1) {
    
        // find appropriate frame
        for (auto itr=frames.begin(); itr!=frames.end(); ++itr) {
            for (int i=0; i<itr->clustersCount; ++i) {
                if (m_d->baselineClusterNo == clusters()[ itr->clustersPos+i ])
                    return itr;
            }
        
        }
    }    
     
    return frames.end();
}

void SalvDbusCheck::breakEventLoop(int retcod)
{
    m_d->eventLoop->exit(retcod);
}

bool SalvDbusCheck::isWaiting() const
{
    return m_d->eventLoop->isRunning();
}

