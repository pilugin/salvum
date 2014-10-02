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
    
    struct FrameDescription
    {
        FrameDescription(int clustersPos_ =0, int clustersCount_ =0, DecodrFrame *frame_ =nullptr, bool accepted_ =false);
        int clustersPos;
        int clustersCount;
        DecodrFrame *frame;
        bool accepted;
    };
    typedef QVector<FrameDescription> FrameDescription_v;
    typedef FrameDescription_v::const_iterator FrameDescription_itr;

signals:
    void skipClusters(int clusterNo, int length);
    void saveResults(const QVector<QPair<int, QByteArray>> &clusters);
    void baselineFrame(const DecodrFrame &frame);

public slots:
    void onFetch(int clusterNo, const QByteArray &cluster);
    void onAccept(const DecodrFrame &frame);
    void onReject(const DecodrFrame &frame);
    
    void onFetchEnd();
    
protected:
    typedef QVector<QPair<int,QByteArray>> Clusters;

    virtual void doAcceptFrame(const Clusters &pendingClusters, const DecodrFrame &frame);
    virtual void doRejectFrame(const Clusters &pendingClusters, const DecodrFrame &frame);
        
    virtual FrameDescription_itr chooseBaseline(const FrameDescription_v &frames) =0;
    
    const Clusters &clusters() const { return mClusters; }   
private:
    void processFetchEnd();
    
    void beginSkipClusters();
    void skipCluster(int cluster);
    void endSkipClusters();
    struct {
        int cluster;
        int len;
        bool flushed;
    } mSkipClustersTmp;

    Clusters mPendingClusters;
    
    /*
    ** 0-1-2-3--------8-9----------4-6-7-----         <-- mClusters
    ** |              |            |         
    ** 0+4+Frame0-----4+2+Frame1---6+3+Frame2         <-- mFrames
    **                          
    **
    ** Meaning:
    ** Frame0 - {0..3}
    ** Frame1 - {8..9}
    ** Frame2 - {4..7}
    */
    Clusters mClusters;   
    QVector<FrameDescription> mFrames;
    bool mPrevAccepted;
    bool mFetchEnd;
};

}

#endif // CORE_CHECK_H
