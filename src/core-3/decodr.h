#ifndef CORE_3_DECODR_H
#define CORE_3_DECODR_H

#include <common/types.h>

namespace Core3 {

template <class DecodrState>
class Decodr
{
public:
    typedef DecodrState State;

    virtual ~Decodr()                               {}
    
    // init 
    virtual void init() =0;
    virtual bool feed(const Common::Cluster &cluster) =0;    //< return true if decodr is Serailizable 
    virtual bool initialized() const =0;
    
    // check
    virtual bool decodOk() const =0;
    virtual bool checkOk() const =0;
    virtual bool end() const =0;
    
    // save+restore
    virtual const State &state() const =0;
    void restore(const State &state_)               { if (state_!=state()) doRestore(state_); }
protected:
    virtual void doRestore(const State &state) =0;
    
};

///////////////////////////////////////////////////

}

#endif
