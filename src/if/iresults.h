#ifndef IRESULTS_H
#define IRESULTS_H

#include <QVector>

class QString;

class IResults
{
public:
    virtual ~IResults() {}

    virtual bool restart(const QString &name) =0;
    virtual void addClusters(const QVector<int> &clusterNos) =0;

    virtual void finilize(bool success) =0;

    virtual QVector<int> clusters() const =0;
};

#endif
