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

typedef Array<Cluster, 1000> Clusters;

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


typedef Array<int, 4000> SkipClusters;

typedef Array< std::pair<int, int>, 20> RewindClusters;

struct Feedback
{
    bool            feedbackNeeded;
    int             completeCount; //< this prop is by recv when he finished the feedback
    SkipClusters    skip;
    RewindClusters  rewind;
};

enum FeedbackSync {
    BCAST = 0,  //< SharedFeedback broadcast condvar
    RECV = 0    //< SharedFeedback reciever condvar
};

typedef IPC::SynchroMem<Feedback, 1, 2> SharedFeedback;

} // ns IPCFetch

#endif // IPCFETCH_COMMON_H