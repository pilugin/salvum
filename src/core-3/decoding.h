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
    bool prevFit = false;
    bool decodAppear = false;

    for (;;) {

        if ( f.atEnd() || d.end() ) {
            if (!decodAppear)
                return false;
            decodAppear=false;

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
            if (!d.checkOk() && !prevFit)
                a.revertClusters();
            else
                a.setNewState(d.state(), d.checkOk());

            if (!initialized && d.initialized() && d.checkOk()) {
                f.skipClusters( a.baseline(f.lastFetched().first) );
                initialized = true;
            }
        } else
            a.revertClusters();

        prevFit = d.checkOk();
        decodAppear = decodAppear || prevFit;

        if (!d.checkOk()) {
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
