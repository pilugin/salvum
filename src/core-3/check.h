#ifndef CORE_3_CHECK_H
#define CORE_3_CHECK_H

#include <common/types.h>

namespace Core3 {

template <class> class Archive;

template <class DecodrState>
class Check
{
public:
    typedef QPair<int, DecodrState> SelectResult;

    virtual ~Check() {}

    virtual SelectResult select(const Archive<DecodrState> &archive) =0;
};

}

#endif // CORE_3_CHECK_H