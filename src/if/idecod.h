#ifndef IDECOD_H
#define IDECOD_H

#include <QVector>

class IFetch;

class IDecod
{
public:
    virtual ~IDecod() {}

    virtual bool restart(IFetch *fetch) =0; //< false on error
    virtual bool decodeCluster() =0; //< false on error; one call of decod calls IFetch->fetch() once

    virtual void revert() =0; //< reverts internal state to the state before decodeCluster()

    virtual QVector<int> usedClusters() const =0;

    virtual bool done() const =0; //< true on EOI (no error)

};

#endif // IDECOD_H
