#include <util/simplecheck.h>
#include <QtDebug>

using namespace Core;

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

Check::FrameDescription_itr SimpleCheck::chooseBaseline(const Check::FrameDescription_v &frames)
{
    if (mVerbose) {
        qDebug()<<"Check::chooseBaseline:";
        for (int i=0; i<frames.size(); ++i)
            qDebug("\t%X-{ %08X ~ %08X}", i, clusters()[frames[i].clustersPos].first, clusters()[frames[i].clustersPos + frames[i].clustersCount -1].first );

        qDebug()<<"\t\tsize="<<frames.size();
    }

    return frames.begin() + frames.size() - 1;
}
