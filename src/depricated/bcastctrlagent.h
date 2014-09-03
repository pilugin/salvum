#ifndef IPCFETCH_BCASTCTRLAGENT_H
#define IPCFETCH_BCASTCTRLAGENT_H

#include <ipcfetch/bcastctrlcommon.h>
#include <string>

namespace IPCFetch {

class BcastCtrlAgent
{
public:
    BcastCtrlAgent(const char *shmem);
    ~BcastCtrlAgent();

    bool isValid() const { return mData != nullptr; }

    bool waitForRequest(); //< return true on incoming message, false on timeout

    const BcastCtrlRequest &request() const { return mData->request; }
    BcastCtrlResponse &response()           { return mData->response; }

    void sendResponse();
private:
    const std::string       mShmemName;
    SharedBcastCtrlData     *mData;
};

} // ns IPCFetch

#endif // IPCFETCH_BCASTCTRLAGENT_H
