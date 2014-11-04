#ifndef UTIL_SIMPLE_CHECK_H
#define UTIL_SIMPLE_CHECK_H

#include <core-3/check.h>
#include <core-3/archive.h>

template <class DecodrState>
class SimpleCheck : public Core3::Check<DecodrState>
{
public:
    typedef Core3::Check<DecodrState> Parent;
    typename Parent::SelectResult select(const Core3::Archive<DecodrState> &archive)
    {
        typename Core3::Archive<DecodrState>::ClusterStored cs;
        archive.lastOkState(&cs);
        return typename Parent::SelectResult(cs.cluster.first, cs.decodrState);
    }

};

#endif // UTIL_SIMPLE_CHECK_H
