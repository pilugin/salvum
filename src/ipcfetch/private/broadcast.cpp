#include <ipcfetch/broadcast.h>
#include <util/ilog.h>

#include <QVector>
#include <QByteArray>
#include <QFile>
#include <cctype>

using namespace Log;
using namespace IPC;
using namespace RdWr;
using namespace Core;

namespace IPCFetch {

Broadcast::Broadcast(const char *shmemName, Fetch *fetch)
: Writer< BroadcastMessage >(shmemName)
, mFetch(fetch)
{
    sharedData().rewind = Fetch::InvalidClusterNo;
}

void Broadcast::write(Fetch *fetch)
{
    if (fetch)
        mFetch = fetch;

    mFetch->rewind(0);
    mFetch->fastfwd();

    Writer< BroadcastMessage > ::write();
}

bool Broadcast::prepare(BroadcastMessage &message)
{
    if ( message.rewind != Fetch::InvalidClusterNo && sharedMem().regCount == 1) {
        Msg("BCAST:prepare rewind(%08X)\n", message.rewind );
        mFetch->rewind( message.rewind );
        message.rewind = Fetch::InvalidClusterNo;
    }

    if ( mFetch->atEnd() ) {
        Msg("BCAST:prepare atEnd\n");

        message.status = AtEnd;
        message.clusters.clear();

    } else {
        message.clusters.clear();
        
        int fetchClusterNo;
        QByteArray fetchCluster;
        while (!message.clusters.full()  && !mFetch->atEnd() ) {

            mFetch->fetch(fetchClusterNo, fetchCluster);
            if (fetchClusterNo == Fetch::InvalidClusterNo || fetchCluster.size() == 0)
                break;

            message.clusters.push_back_empty();
            message.clusters.back().cluster.set( fetchCluster.data(), fetchCluster.size() );
            message.clusters.back().clusterNo = fetchClusterNo;
        }

        message.status = (message.clusters.size() > 0) ? InProgress : AtEnd;

        Msg("BCAST:prepare status=%s\n", message.status == InProgress ? "InProgress" : "AtEnd" );
    }

    return true;
}



void Broadcast::postRead(const BroadcastMessage &message)
{
    if (message.status == AtEnd) {

        Msg("BCAST:postRead[AtEnd] rewind+fastfwd\n");
        mFetch->rewind(0);
        mFetch->fastfwd();
    }
}

//////

QByteArray Broadcast::dumpStats()
{
    mStats.clear();
    interrupt(DumpStats);
    return mStats;
}

QPair<bool, QString> Broadcast::saveBitmap(const QString &filename)
{
    mSaveBitmapFilename = filename;
    interrupt(SaveBitmap);
    return qMakePair(mSaveBitmapSuccess, mSaveBitmapError);
}

bool Broadcast::processInternalMsg(int internalMsg)
{
    switch (internalMsg) {
    case DumpStats:
        {
            QByteArray bm = mFetch->bitmap();
            QVector<int> stats(256, 0);
            for (int i=0; i<bm.size(); ++i)
                ++stats[ bm[i] ];
            // print
            char tmpstr[64];
            QByteArray statsText;
            snprintf( tmpstr, sizeof(tmpstr)-1, "REG:%d\n", sharedMem().regCount);
            statsText.append(tmpstr);
            for (int i=0; i<stats.size(); ++i) 
                if (stats[i] > 0) {
                    char c = i;
                    if ( isascii(c) )
                        snprintf( tmpstr, sizeof(tmpstr)-1, "%c=%d\n", c, stats[i] );
                    else 
                        snprintf( tmpstr, sizeof(tmpstr)-1, "0x%X=%d\n", i, stats[i] );
                        
                    statsText.append(tmpstr);                
                }
            mStats = statsText;
        }
        return true;
    case SaveBitmap:
        {
            QByteArray bm = mFetch->bitmap();
            QFile out(mSaveBitmapFilename);
            if (! out.open(QFile::WriteOnly | QFile::Truncate) ) {
                mSaveBitmapSuccess = false;
                mSaveBitmapError = out.errorString();

            } else {
                int written = out.write(bm);
                mSaveBitmapSuccess = (written == bm.size());
                if (!mSaveBitmapSuccess) 
                    mSaveBitmapError.sprintf("Could not write whole bitmap file. Only %d/%d written", written, bm.size() );
            }
        }
        return true;
    default:
        return Super::processInternalMsg(internalMsg);
    }
}

} // ns IPCFetch
