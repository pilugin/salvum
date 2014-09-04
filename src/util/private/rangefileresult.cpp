#include "util/rangefileresult.h"
#include "util/ilog.h"
#include <QDir>

RangeFileResult::RangeFileResult(const QString &dir, QObject *parent) : Result(parent), mDir(dir)
{
    if (!QDir().mkpath(mDir))
        Log::Msg("Failed to mkdir %s\n", mDir.toAscii().data());
}

void RangeFileResult::restart(const QString &name)
{
    mName = name;
    mFile.setFileName( QString("%1/%2.clusters").arg(mDir, name) );
    
    if (!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) 
        Log::Msg("Failed to create file %s\n", mFile.fileName().toAscii().data());
    
}

void RangeFileResult::addClusters(int clusterNo, int length)
{
    mFile.write(QString().sprintf("%X %X\n", clusterNo, clusterNo +length -1).toUtf8());
}

void RangeFileResult::finalize(bool success)
{
    mFile.close();

    if (!success) {
        if (!QFile::rename(mFile.fileName(), QString("%1/%2.bad.clusters").arg(mDir, mName)))
            Log::Msg("Failed to move file %s\n", mFile.fileName().toAscii().data());
    }
}
