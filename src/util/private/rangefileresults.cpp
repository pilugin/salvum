#include "util/rangefileresults.h"
#include "util/ilog.h"
#include <QDir>

RangeFileResults::RangeFileResults(const QString &dir, QObject *parent) : Result(parent), mDir(dir)
{
    if (!QDir().mkpath(mDir))
        Log::Msg("Failed to mkdir %s\n", mDir.toAscii().data());
}

void RangeFileResults::restart(const QString &name)
{
    mName = name;
    mFile.setFileName( QString("%1/%2.clusters").arg(mDir, name) );
    
    if (!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mFile.fileName().toAscii().data());
    
}

void RangeFileResults::addClusters(int clusterNo, int length)
{
    mFile.write(QString().sprintf("%X %X\n", clusterNo, clusterNo +length -1).toUtf8());
}

void RangeFileResults::finilize(bool success)
{
    mFile.close();

    if (!success) {
        if (!QFile::rename(mFile.fileName(), QString("%1/%2.bad.clusters").arg(mDir, mName)))
            Log::Msg("Failed to move file %s\n", mFile.fileName().toAscii().data());
    }
}
