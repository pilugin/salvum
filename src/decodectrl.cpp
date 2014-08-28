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
    Session( QString::number(clusterNo, 16) );
    results->restart( QString::number(clusterNo, 16) );

    qDebug("%08X start", clusterNo);
    if (rewind(fetch, clusterNo)   &&  startDecode(decodr, fetch, results) ) {
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

//            fetch->skip( results->clusters() );
            rewind(fetch);

            afterStart = false;
        }

    }

    results->finilize(decodr->done());
    Msg("Finished. %X %s", clusterNo, decodr->done() ? "COMPLETE" : "incomplete");
    return decodr->done();
}

bool DecodeCtrl::rewind(IFetch *fetch, int clusterNo)
{
    qDebug("rewind");
    bool rv = true;

    rv = fetch->rewind(clusterNo);
    if (rv) {
	if (clusterNo == 0) 
    	    fetch->fastfwd();

        Msg("Fetch rewind(%d) %s\n", clusterNo, clusterNo==0 ? "+ ffwd()" : "");
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
//    results->addClusters( decodr->usedClusters() );

    return true;
}

bool DecodeCtrl::proceedDecode(IDecod *decodr, IFetch *fetch, IResults *results)
{
    qDebug("proceedDecode");
    do {
        if ( decodr->decodeCluster() )
;//            results->addClusters( decodr->usedClusters() );

        else {
            decodr->revert();
            fetch->fastfwd();
        }
    } while ( !fetch->atEnd()  &&  !decodr->done() );

    return decodr->done();
}
