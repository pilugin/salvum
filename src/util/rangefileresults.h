#ifndef RANGEFILERESULTS_H
#define RANGEFILERESULTS_H

#include <core/result.h>
#include <QFile>

class RangeFileResults : public Result
{
public:
    RangeFileResults(const QString &dir =".", QObject *parent =nullptr);

    void restart(const QString &name);
    void addClusters(int clusterNo, int length);
    void finilize(bool success);

private:
    const QString mDir;
    QString mName;
    QFile mFile;
};

#endif // RANGEFILERESULTS_H
