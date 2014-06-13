#ifndef IPCFETCH_BCASTCTRLAGENT_H
#define IPCFETCH_BCASTCTRLAGENT_H

#include <ipcfetch/bcastctrl.h>
#include <string>

namespace IPCFetch {

class BcastCtrlAgent
{
public:
    BcastCtrlAgent(const char *shmem);
    ~BcastCtrlAgent();

    bool isValid() const { return mData != nullptr; }
    void process();
private:
    const std::string       mShmemName;
    SharedBcastCtrlData     *mData;
};

} // ns IPCFetch

#endif // IPCFETCH_BCASTCTRLAGENT_H
