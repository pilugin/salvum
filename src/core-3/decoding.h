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

    bool initialized = false;
    bool cansync = false;
    bool fit = false;

    for (;;) {

        if ( f.atEnd() || d.end() ) {
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

        do {
            cansync = d.feed( f.fetch() );
            a.addNewCluster( f.lastFetched() );
        } while ( !cansync );

        if (d.decodOk()) {
            fit = d.checkOk();
            a.setNewState(d.state(), fit);
            if (!initialized && d.initialized() && fit) {
                f.skipClusters( a.baseline(f.lastFetched()) );
                initialized = true;
            }
        }
        if (!fit) {
            if (!d.initialized())
                return false;
            d.restore(a.lastOkState());
            f.fastfwd();
        }
    }

    Q_ASSERT(false);
    return true;
}

}

#endif // CORE_3_DECODING_H