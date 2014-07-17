#include "bench.h"
#include "jpeg/advancedchecker.h"
#include "if/isettings.h"
#include "if/ilog.h"

#include <QtDebug>

using namespace Settings;
using namespace Log;

namespace Jpeg {

Bench::Bench() : mTestee(0)
{
}

bool Bench::init(ICheck *testee, const QString &clustersPath)
{
    mTestee = testee;
    mCheckRelevances.clear();
    mInput.setFileName(clustersPath);
    mOffset = 0;
    if (!mInput.open(QFile::ReadOnly)) {
        qDebug()<<"ERR:"<<clustersPath<<mInput.errorString();
        return false;
    }

    return true;
}

//////

void Bench::fetch(int &clusterNo, QByteArray &cluster)
{
    Msg("\nF");

    if (!mInput.seek(mOffset * Get(ClusterSize).toInt())) {
        qDebug() << "ERR:"<<mInput.fileName()<<mInput.errorString();
        clusterNo = InvalidClusterNo;
        cluster.clear();
    }
    cluster = mInput.read(Get(ClusterSize).toInt());
    clusterNo = mOffset;

    mCheckRelevances.push_back( qMakePair(mOffset, -1.) );

    ++mOffset;
}

bool Bench::atEnd() const
{
    return (mOffset * Get(ClusterSize).toInt()) >= mInput.size();
}

///////

bool Bench::check(const QImage &image, int blockBegin, int blockEnd, double *relevance)
{
    double localRelevance = 0.;
    bool res = mTestee ? mTestee->check(image, blockBegin, blockEnd, &localRelevance) : false;

    if (relevance)
        *relevance = localRelevance;

    mCheckRelevances.back().second = localRelevance;
    return res;
}

double Bench::minRelevance() const
{
    return mTestee ? mTestee->minRelevance() : 1.;
}

} // eons Jpeg


