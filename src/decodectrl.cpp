#include "decodectrl.h"

#include "if/idecod.h"
#include "if/ifetch.h"
#include "if/iresults.h"
#include "if/ilog.h"

#include <QtDebug>

using namespace Log;

DecodeCtrl::DecodeCtrl()
{
}





bool DecodeCtrl::decode(IDecod *decodr, IFetch *fetch, IResults *results, int clusterNo)
{
    return decode(decodr, fetch, results, QList<QPair<int, int>>() << qMakePair(clusterNo, (int)IFetch::InvalidClusterNo) );
}

bool DecodeCtrl::decode(IDecod *decodr, IFetch *fetch, IResults *results, const QList<QPair<int, int>> &rewinds)
{
    int clusterNo = rewinds.isEmpty() ? IFetch::InvalidClusterNo : rewinds.front().first;
    Session( QString::number(clusterNo, 16) );
    results->restart( QString::number(clusterNo, 16) );

    mRewinds = rewinds;

    qDebug("%08X start", clusterNo);
    if (rewind(fetch)   &&  startDecode(decodr, fetch, results) ) {
        bool afterStart = true;
        int prevClustersFit = results->clusters().size();

        for (;;) {
            bool rv = proceedDecode(decodr, fetch, results);
            if (rv) //< done!
                break;

            int clustersFit = results->clusters().size();
            if (clustersFit == prevClustersFit && !afterStart) //< no new clusters since prev decode; exit :(
                break;
            prevClustersFit = clustersFit;

            fetch->skip( results->clusters() );
            rewind(fetch);

            afterStart = false;
        }

    }

    results->finilize(decodr->done());
    Msg("Finished. %X %s", clusterNo, decodr->done() ? "COMPLETE" : "incomplete");
    return decodr->done();
}

bool DecodeCtrl::rewind(IFetch *fetch)
{
    qDebug("rewind");
    bool rv = true;
    if (mRewinds.isEmpty()) {
        fetch->rewind(0);
        fetch->fastfwd();

        Msg("Fetch rewind(0) + ffwd()\n");
    } else {
        rv = fetch->rewind(mRewinds.front().first, mRewinds.front().second);

        Msg("Fetch rewind(%X, %X): %s\n", mRewinds.front().first, mRewinds.front().second, rv ? "OK" : "Fail");
        mRewinds.pop_front();
    }

    return rv;
}



bool DecodeCtrl::startDecode(IDecod *decodr, IFetch *fetch, IResults *results)
{
    qDebug("startDecode");
    bool rv = decodr->restart(fetch);
    Msg("Decodr restart()=%s\n", rv ? "OK" : "Fail");
    if (!rv)
        return false;
    results->addClusters( decodr->usedClusters() );

    return true;
}

bool DecodeCtrl::proceedDecode(IDecod *decodr, IFetch *fetch, IResults *results)
{
    qDebug("proceedDecode");
    do {
        if ( decodr->decodeCluster() )
            results->addClusters( decodr->usedClusters() );

        else {
            decodr->revert();
            fetch->fastfwd();
        }
    } while ( !fetch->atEnd()  &&  !decodr->done() );

    return decodr->done();
}
