#ifndef RANGEFILERESULTS_H
#define RANGEFILERESULTS_H

#include <QFile>
#include <common/types.h>

class RangeFileResult
{
public:
    RangeFileResult(const QString &dir =".");

    void restart(const QString &clustersName ="clusters", const QString &dataName ="data");
    void addCluster(const Common::Cluster &cluster);
    void finalize(bool success);

protected:
    const QString mDir;
    QFile mFile;
    QFile mDataFile;

    void store(int beginCluster);
    void flush();
    struct {
        int beginCluster;
        int len;
    } mClusterRange;
};

#endif // RANGEFILERESULTS_H
