#ifndef BCASTCTRL_BCASTREMOTECTRL_H
#define BCASTCTRL_BCASTREMOTECTRL_H

#include <ipcfetch/bcastctrlcommon.h>

class BcastRemoteCtrl 
{
public:
    BcastRemoteCtrl(const char *shmem);
    ~BcastRemoteCtrl();

    bool isValid() const { return mData != nullptr; }

    IPCFetch::BcastCtrlRequest &request() const { return mData->request; }
    IPCFetch::BcastCtrlResponse &response()     { return mData->response; }

    bool acquire();
    bool exchange();
    bool free();

private:

    IPCFetch::SharedBcastCtrlData *mData;
};

#endif