#ifndef CORE_CHECK_H
#define CORE_CHECK_H

#include "decodr.h"
#include <QPair>
#include <QVector>

class Check : public QObject
{
    Q_OBJECT
public:
    Check(QObject *parent =nullptr);

signals:
    void skipClusters(int clusterNo, int length);
    void baselineFrame(const DecodrFrame &frame);

public slots:
    void onFetch(int clusterNo);
    void onAccept(const DecodrFrame &frame);
    void onReject();
    
    void onFetchEnd();
    
protected:    
    virtual void doAcceptFrame(const QVector<int> &pendingClusters, const DecodrFrame &frame);
    virtual void doRejectFrame(const QVector<int> &pendingClusters);
    
    struct FrameDescription
    {
        FrameDescription(
                QVector<int>::const_iterator clustersBegin_ =QVector<int>::const_iterator(),
                QVector<int>::const_iterator clustersEnd_   =QVector<int>::const_iterator(),
                DecodrFrame *frame_                         =nullptr
        );
        QVector<int>::const_iterator clustersBegin;
        QVector<int>::const_iterator clustersEnd;        
        DecodrFrame *frame;        
    };
    typedef QVector<FrameDescription> FrameDescription_v;
    typedef FrameDescription_v::const_iterator FrameDescription_itr;
    
    virtual const FrameDescription &chooseBaseline(const FrameDescription_v &frames) =0;
   
private:
    QVector<int> mPendingClusters;
    
    /*
    ** 0-1-2-3--------8-9--------4-6-7            <-- mAcceptedClusters
    **       |          |            |
    ** ------3+Frame0---9+Frame1-----7+Frame2     <-- mFrames
    **
    ** Meaning:
    ** Frame0 - {0..3}
    ** Frame1 - {8..9}
    ** Frame2 - {4..7}
    */
    QVector<int> mAcceptedClusters;
    QVector<QPair<int /*clusterNo*/, DecodrFrame *>> mFrames;
};

#endif // CORE_CHECK_H
