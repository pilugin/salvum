#include "util/rangefileresult.h"
#include "util/ilog.h"
#include <functional>
#include <QDir>
#include <QtDebug>

using namespace Core;

RangeFileResult::RangeFileResult(const QString &dir, QObject *parent) : Result(parent), mDir(dir)
{
    if (!QDir().mkpath(mDir))
        Log::Msg("Failed to mkdir %s\n", mDir.toUtf8().data());
}

void RangeFileResult::restart(const QString &name)
{
    mName = name;
    mFile.setFileName(      QString("%1/%2.clusters").arg(mDir, name) );
    mDataFile.setFileName(  QString("%1/%2.data").arg(mDir, name) );
    
    if (!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mFile.fileName().toUtf8().data());
    if (!mDataFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mDataFile.fileName().toUtf8().data());
}

void RangeFileResult::addClusters(const QVector<QPair<int, QByteArray>> &clusters)
{
    int len=0;
    int beginCluster=0;
    bool flushed=true;
    
    std::function<void(int)> store = [&](int c) {  
        beginCluster=c;
        len=1;
        flushed=false;
    };
    std::function<void()> flush = [&]() {
        if (mFile.write(QString().sprintf("%X %X\n", beginCluster, beginCluster +len -1).toUtf8()) == -1)
            Log::Msg("Failed to Result::addClusters. %s", mFile.errorString().toUtf8().data());
        flushed=true;            
    };
    
    for (int i=0; i<clusters.size(); ++i) {
        if (flushed) {           
            store(clusters[i].first);
        } else {
            if ((beginCluster + len) == clusters[i].first) 
                ++len;
            else {
                flush();
                store(clusters[i].first);
            }                
        }
        
        if (mDataFile.write(clusters[i].second) == -1)
            Log::Msg("Result: Failed to write cluster. %s", mDataFile.errorString().toUtf8().data() );
    }
    flush();
}

void RangeFileResult::finalize(bool success)
{
    mFile.close();
    mDataFile.close();

    if (!success) {
        if (!QFile::rename(mFile.fileName(), QString("%1/%2.bad.clusters").arg(mDir, mName)))
            Log::Msg("Failed to move file %s\n", mFile.fileName().toUtf8().data());
    }
}
