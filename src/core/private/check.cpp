#include <core/check.h>
#include <QtDebug>

using namespace Common;

namespace Core {

Check::Check(QObject *parent) : QObject(parent), mPrevAccepted(false)
{
}

void Check::onFetch(int clusterNo, const QByteArray &cluster)
{
    mPendingClusters.push_back(qMakePair(clusterNo, cluster));
    mFetchEnd = false;
}

void Check::onAccept(const DecodrFrame &frame)
{
    doAcceptFrame(mPendingClusters, frame);

    mClusters += mPendingClusters;
    if (mClusters.size() > 0) {
        int pos = mFrames.size()==0 ? 0 : mFrames.back().clustersPos + mFrames.back().clustersCount;
        mFrames.push_back( FrameDescription(pos, mPendingClusters.size(), frame.clone(), true) );
    }        

    mPendingClusters.clear();    
    
    if (mFetchEnd)
        processFetchEnd();
        
    mPrevAccepted = true;        
}

void Check::onReject(const DecodrFrame &frame)
{
    doRejectFrame(mPendingClusters, frame);
    
    if (mPrevAccepted && frame.decodeOk()) {
        mClusters += mPendingClusters;
        if (mClusters.size() > 0) {
            mFrames.push_back( 
                FrameDescription( (mFrames.back().clustersPos + mFrames.back().clustersCount), 
                                mPendingClusters.size(), frame.clone(), false) 
                );
        }
    }
    
    mPendingClusters.clear();
    
    if (mFetchEnd)
        processFetchEnd();
        
    mPrevAccepted = false;        
}

void Check::onFetchEnd()
{
    mFetchEnd = true;
}

void Check::onDecodrEnd()
{
    processFetchEnd();
}

void Check::processFetchEnd()
{
    FrameDescription_itr chosenOne = chooseBaseline( mFrames );
    if (chosenOne != mFrames.end()) {
    
//    qDebug()<<"CLUSTERS"<<mClusters;
    
    // interpret result; emit skipClusters & baselineFrame
        beginSkipClusters();
    
        Clusters results;
        for (auto f=mFrames.begin(); f <= chosenOne; ++f) {
//        qDebug()<<"FRAME"<<f->clustersPos<<"->"<<mClusters[f->clustersPos]<<"  "<<f->clustersCount;
    
            if (!f->accepted && f!=chosenOne)
                continue;
            
            for (int i=0; i<f->clustersCount; ++i) {
//            qDebug()<<"call SKIP CLUSTERS "<<(f->clustersPos+i);
                skipCluster( mClusters[f->clustersPos+i].first );
                results.append( mClusters[f->clustersPos+i] );
            }
        }
        emit saveResults(results);
        endSkipClusters();    
        emit baselineFrame(*chosenOne->frame);        
    }
    
    // clear all
    for (auto frame : mFrames)
        delete frame.frame;
    mFrames.clear();
    mClusters.clear();
}

void Check::beginSkipClusters()
{
    mSkipClustersTmp.flushed = true;
}

void Check::skipCluster(int cluster)
{
//    qDebug()<<"Check::skipCluster  "<<cluster;
    if (!mSkipClustersTmp.flushed 
        && (cluster == (mSkipClustersTmp.cluster + mSkipClustersTmp.len) )) {
        
        ++mSkipClustersTmp.len;
        return;        
    }
    
    if (!mSkipClustersTmp.flushed)
        endSkipClusters();
        
    mSkipClustersTmp.flushed = false;
    mSkipClustersTmp.cluster = cluster;
    mSkipClustersTmp.len = 1;        
}

void Check::endSkipClusters()
{
    if (mSkipClustersTmp.flushed)
        return;
        
//    qDebug()<<"Check:: emit skipClusters:"<<mSkipClustersTmp.cluster<<", "<< mSkipClustersTmp.len;
    emit skipClusters(mSkipClustersTmp.cluster, mSkipClustersTmp.len);
    mSkipClustersTmp.flushed = true;        
}

void Check::doAcceptFrame(const Clusters &/*pendingClusters*/, const DecodrFrame &/*frame*/)
{
}

void Check::doRejectFrame(const Clusters &/*pendingClusters*/, const DecodrFrame &/*frame*/)
{
}

//////////////////////////////////////////////////////////////////////

Check::FrameDescription::FrameDescription(int clustersPos_, int clustersCount_, DecodrFrame *frame_, bool accepted_)
: clustersPos(clustersPos_)
, clustersCount(clustersCount_)
, frame(frame_)
, accepted(accepted_)
{
}

}
