#ifndef IPCFETCH_BCASTCTRL_H
#define IPCFETCH_BCASTCTRL_H

#include <util/array.h>
#include <util/ipc.h>

namespace IPCFetch {

enum BcastCtrlCommand
{
    Noop,
    Pause,
    Resume,
    Exit,
    Brute,
    DumpStats,
    SaveBitmap
};

struct BcastCtrlRequest
{
    BcastCtrlRequest() : command(Noop) {}

    BcastCtrlCommand    command;
    Array<char, 1024>   strparam;
    bool                boolparam;
};

struct BcastCtrlResponse
{
    bool                success;
    Array<char, 1024>   str; //< in case when success=false str is used to describe an error
};

struct BcastCtrlData
{
    enum { Free=-1 };

    BcastCtrlData() : id(Free), generator(0) {}

    BcastCtrlRequest    request;
    BcastCtrlResponse   response;

    int                 id;
    int                 generator;
};

typedef IPC::SynchroMem< BcastCtrlData, 1, 1> SharedBcastCtrlData;

} // ns IPCFetch

#endif // IPCFETCH_BCASTCTRL_H
