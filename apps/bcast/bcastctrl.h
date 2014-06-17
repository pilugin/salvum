#ifndef BCAST_BCASTCTRL_H
#define BCAST_BCASTCTRL_H

#include <QString>

namespace IPCFetch {
class BcastCtrlAgent;
class Broadcast;
struct BcastCtrlRequest;
struct BcastCtrlResponse;
}
class DeviceMapFetch;

class BcastCtrl
{
public:
    BcastCtrl(const char *bcast_shmem, const char *bcastfb_shmem, const char *ctrl_shmem, 
                const QString &deviceFile, const QString &mapFile);
    ~BcastCtrl();

    bool isValid() const;

    bool exec();

    

private:

    bool processRq(const IPCFetch::BcastCtrlRequest &rq, IPCFetch::BcastCtrlResponse &rs); //< return false on exit

    class Thread;

    IPCFetch::BcastCtrlAgent    *mCtrl;
    IPCFetch::Broadcast         *mBcast;
    DeviceMapFetch              *mFetch;
    Thread                      *mThread;

};

#endif // BCAST_BCASTCTRL_H