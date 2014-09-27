#ifndef IPCFETCH_COMMON_H
#define IPCFETCH_COMMON_H

#include <utility>
#include <util/array.h>
#include <util/ipc.h>

namespace IPCFetch {

typedef Array<char, 4096> ClusterData; //< TBD: parameterize 4096 somehow

struct Cluster 
{
    int         clusterNo;  
    ClusterData cluster; 
};

typedef Array<Cluster, 1024> Clusters;

enum BroadcastStatus
{
    InProgress,
    AtEnd,
    About2Quit
};

struct BroadcastMessage
{
    BroadcastStatus status;
    Clusters        clusters;
};

} // ns IPCFetch

#endif // IPCFETCH_COMMON_H