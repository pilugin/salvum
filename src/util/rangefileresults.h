#ifndef RANGEFILERESULTS_H
#define RANGEFILERESULTS_H

#include <core/result.h>
#include <QFile>

class RangeFileResults : public Result
{
public:
    RangeFileResults(const QString &dir =".");

    bool restart(const QString &name);
    void addClusters(const QVector<int> &clusterNos);

    void finilize(bool success);

    QVector<int> clusters() const;

private:
    void addCluster(int clusterNo);

    const QString mDir;
    QString mName;
    QFile mFile;
    QVector<int> mClusters;
    int mLastAddedCluster;
    static const int sEmptyLastAddedCluster = -1;
};

#endif // RANGEFILERESULTS_H
