#ifndef CORE_3_DECODING_H
#define CORE_3_DECODING_H

#include <core-3/fetch.h>
#include <core-3/archive.h>
#include <core-3/decodr.h>
#include <core-3/check.h>

namespace Core3 {

template <class DecodrState>
bool decoding(int clusterNo, Fetch &f, Decodr<DecodrState> &d, Check<DecodrState> &c, Archive<DecodrState> &a)
{
    f.rewind(clusterNo);
    d.init();
    while (d.initialized()) {
        d.feed( f.fetch() );
        a.addInitCluster( f.lastFetched() );
    }

    if (!d.decodOk())
        return false;

    a.setNewState( d.state(), true );

    while (!d.end()) {
        if (f.atEnd() || d.end()) {
            typename Check<DecodrState>::SelectResult selected = c.select( a );
            if (selected.first == Common::InvalidClusterNo)
                return false;

            f.skipClusters( a.baseline(selected.first) );
            d.restore( selected.second );

            if (d.end())
                return true;

            f.rewind();
            f.fastfwd();
        }


        d.decode(f.fetch());
        a.addNewCluster(f.lastFetched());

        bool fit = false;
        if (d.decodOk()) {
            fit = d.checkOk();
            a.setNewState(d.state(), fit);
        }
        if (!fit) {
            d.restore(a.lastOkState());
            f.fastfwd();
        }
    }

    return true;
}

}

#endif // CORE_3_DECODING_H