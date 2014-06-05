#ifndef IPCFETCH_FEEDBACKRESULTS_H
#define IPCFETCH_FEEDBACKRESULTS_H

#include <rangefileresults.h>
#include <ipcfetch/common.h>

namespace IPCFetch {

class FeedbackResults : public RangeFileResults
{
public:
    FeedbackResults(const char *shmemName, const QString &dir =".");
    ~FeedbackResults();

    bool restart(const QString &session);
    void addClusters(const QVector<int> &clusterNos);
    void finilize(bool success);

    bool isValid() const { return mFeedback != nullptr; }

private:
    void sendClusters();

    QVector<int> mFreshClusters;
    static const int sClustersToSend = 100;

    SharedFeedback *mFeedback;
};

} // ns IPCFetch

#endif // IPCFETCH_FEEDBACKRESULTS_H