#include <core/check.h>

namespace Core {

Check::Check(QObject *parent) : QObject(parent), mPrevAccepted(false)
{
}

void Check::onFetch(int clusterNo)
{
    mPendingClusters.push_back(clusterNo);        
    mFetchEnd = false;
}

void Check::onAccept(const DecodrFrame &frame)
{
    doAcceptFrame(mPendingClusters, frame);

    mAcceptedClusters += mPendingClusters;
    if (mAcceptedClusters.size() > 0)
        mFrames.push_back( qMakePair<int,DecodrFrame*>(mAcceptedClusters.back(), frame.clone()) );

    mPendingClusters.clear();    
    
    if (mFetchEnd)
        processFetchEnd();
        
    mPrevAccepted = true;        
}

void Check::onReject(const DecodrFrame &frame)
{
    doRejectFrame(mPendingClusters, frame);
    mPendingClusters.clear();
    
    if (mFetchEnd)
        processFetchEnd();
        
    mPrevAccepted = false;        
}

void Check::onFetchEnd()
{
    mFetchEnd = true;
}

void Check::processFetchEnd()
{
    // prepare params for chooseBaseline
    QVector<FrameDescription> frames;
    
    auto beginItr   = mAcceptedClusters.constBegin();
    auto endItr     = beginItr;
    
    for (auto frameItr = mFrames.begin();   frameItr!=mFrames.end()  
                                            && endItr!=mAcceptedClusters.end(); ++frameItr) {
    
        while ( endItr != mAcceptedClusters.end()   
                && *endItr!=frameItr->first)
            ++endItr;
            
        frames.push_back(FrameDescription(beginItr, ++endItr, frameItr->second));
        beginItr = endItr;
    }
    
    // call it
    const FrameDescription &chosenOne = chooseBaseline( frames );
    
    // interpret result; emit skipClusters & baselineFrame
    beginItr = mAcceptedClusters.begin();
    endItr = chosenOne.clustersEnd;

    while (beginItr != endItr) {
        int clusterNo = *beginItr;
        int length = 0;
        
        while ( beginItr != endItr
                && (*beginItr == (clusterNo + length))  ) {
            ++beginItr;
            ++length;
        }
        
        emit skipClusters(clusterNo, length);
    }
    
    emit baselineFrame(*chosenOne.frame);
    
    // clear all
    for (auto pair : mFrames)
        delete pair.second;
    mFrames.clear();
    mAcceptedClusters.clear();
}




void Check::doAcceptFrame(const QVector<int> &/*pendingClusters*/, const DecodrFrame &/*frame*/)
{
}

void Check::doRejectFrame(const QVector<int> &/*pendingClusters*/, const DecodrFrame &/*frame*/)
{
}

//////////////////////////////////////////////////////////////////////

Check::FrameDescription::FrameDescription(
                QVector<int>::const_iterator clustersBegin_,
                QVector<int>::const_iterator clustersEnd_,
                DecodrFrame *frame_,
                bool accepted_
)
: clustersBegin(clustersBegin_)
, clustersEnd(clustersEnd_)
, frame(frame_)
, accepted(accepted_)
{
}

}
