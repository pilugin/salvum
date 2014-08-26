#ifndef IFETCH_H
#define IFETCH_H

#include <QByteArray>
#include <QVector>

class IFetch
{
public:
    enum { InvalidClusterNo =-1 };

    virtual ~IFetch() {}

    virtual bool rewind(int clusterNo =0) =0;

    virtual void skip(int clusterNo, int length) =0;

    virtual void fetch(int &clusterNo, QByteArray &cluster) =0;

    virtual void fastfwd() =0;

    virtual bool atEnd() const =0;

    virtual QByteArray bitmap() const { return QByteArray(); }
};

#endif // IFETCH_H
