#ifndef CORE_CHECK_H
#define CORE_CHECK_H

#include <core/decodr.h>
#include <QPair>
#include <QVector>

namespace Core {

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
    void onReject(const DecodrFrame &frame);
    
    void onFetchEnd();
    
protected:    
    virtual void doAcceptFrame(const QVector<int> &pendingClusters, const DecodrFrame &frame);
    virtual void doRejectFrame(const QVector<int> &pendingClusters, const DecodrFrame &frame);
    
    struct FrameDescription
    {
        FrameDescription(
                QVector<int>::const_iterator clustersBegin_ =QVector<int>::const_iterator(),
                QVector<int>::const_iterator clustersEnd_   =QVector<int>::const_iterator(),                
                DecodrFrame *frame_                         =nullptr,
                bool accepted_                              =false
        );
        QVector<int>::const_iterator clustersBegin;
        QVector<int>::const_iterator clustersEnd;        
        DecodrFrame *frame;        
        bool accepted;
    };
    typedef QVector<FrameDescription> FrameDescription_v;
    typedef FrameDescription_v::const_iterator FrameDescription_itr;
    
    virtual const FrameDescription &chooseBaseline(const FrameDescription_v &frames) =0;
   
private:
    void processFetchEnd();

    QVector<int> mPendingClusters;
    
    /*
    ** 0-1-2-3--------8-9--------4-6------7-X           <-- mClusters
    ** |              |          |          |
    ** |===Frame0=====|----------|===Frame2=|-X         <-- mFrames
    **                |===Frame1=|                      <--
    **                          
    **
    ** Meaning:
    ** Frame0 - {0..3}
    ** Frame1 - {8..9}
    ** Frame2 - {4..7}
    */
    QVector<int> mClusters;   
    QVector<FrameDescription> mFrames;
    bool mPrevAccepted;
    bool mFetchEnd;
};

}

#endif // CORE_CHECK_H
