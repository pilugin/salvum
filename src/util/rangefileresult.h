#ifndef RANGEFILERESULTS_H
#define RANGEFILERESULTS_H

#include <QFile>
#include <common/types.h>

class RangeFileResult
{
public:
    RangeFileResult(const QString &dir =".");

    void restart();
    void addCluster(const Common::Cluster &cluster);
    void finalize(bool success);

protected:
    const QString mDir;
    QFile mFile;
    QFile mDataFile;
};

#endif // RANGEFILERESULTS_H
