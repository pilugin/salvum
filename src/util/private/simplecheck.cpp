#include <util/simplecheck.h>
#include <QtDebug>

SimpleCheck::SimpleCheck(bool verbose, QObject *parent)
    : Check(parent), mVerbose(verbose)
{

}

void SimpleCheck::doAcceptFrame(const QVector<int> &pendingClusters, const DecodrFrame &/*frame*/)
{
    if (mVerbose)
        qDebug()<<"Check::doAcceptFrame: "<<pendingClusters;
}

void SimpleCheck::doRejectFrame(const QVector<int> &pendingClusters)
{
    if (mVerbose)
        qDebug()<<"Check::doRejectFrame: "<<pendingClusters;
}

const Check::FrameDescription &SimpleCheck::chooseBaseline(const Check::FrameDescription_v &frames)
{
    if (mVerbose) {
        qDebug()<<"Check::chooseBaseline:";
        for (int i=0; i<frames.size(); ++i)
            qDebug()<<"\t"<<i<<"-{"<<*frames[i].clustersBegin<<" ~ "<<*(frames[i].clustersEnd -1)<<"}";

        qDebug()<<"\t\tsize="<<frames.size();
    }

    return frames.back();
}
