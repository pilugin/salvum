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

    virtual void revert(int steps=1) =0; //< reverts internal state to the state before decodeCluster()

    virtual int historyLength() const =0; //< returns length of the history
    
    struct Context
    {
        QVector<int> clusterNos;
    };
    
    /** returns decode-context of the frame.
        frameNo=0 - latest frame. Top of the stack
    */
    virtual const Context *historyFrame(int frameNo =0) const =0;

    virtual bool done() const =0; //< true on EOI (no error)

};

#endif // IDECOD_H
