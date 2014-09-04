#include <core/check.h>

Check::Check(QObject *parent) : QObject(parent)
{
}

void Check::onFetch(int clusterNo)
{
    mPendingClusters.push_back(clusterNo);    
}

void Check::onAccept(const DecodrFrame &frame)
{
    doAcceptFrame(mPendingClusters, frame);
    
    mAcceptedClusters += mPendingClusters;
    mFrames.push_back( qMakePair<int,DecodrFrame*>(mAcceptedClusters.back(), frame.clone()) );
    
    mPendingClusters.clear();    
}

void Check::onReject()
{
    doRejectFrame(mPendingClusters);
    mPendingClusters.clear();
}

void Check::onFetchEnd()
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
        int length = 1;
        
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

void Check::doRejectFrame(const QVector<int> &/*pendingClusters*/)
{
}

//////////////////////////////////////////////////////////////////////

Check::FrameDescription::FrameDescription(
                QVector<int>::const_iterator clustersBegin_,
                QVector<int>::const_iterator clustersEnd_,
                DecodrFrame *frame_        
)
: clustersBegin(clustersBegin_)
, clustersEnd(clustersEnd_)
, frame(frame_)
{
}
