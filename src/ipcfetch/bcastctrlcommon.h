#ifndef IPCFETCH_BCASTCTRLCOMMON_H
#define IPCFETCH_BCASTCTRLCOMMON_H

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

enum BcastCtrlMsgType
{
    NoMsg,
    Request,
    Response
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

/*
Client-Server interaction.

Client side:
1) client acquires data. if id==Free then use it
2) set id, set request, wake up the server with cond
3) sleep on cond, until response is delivered. Your response is when id==yourId && command==Response

Server side:
1) wait on cond until command!=Noop
2) process command, write response data, set command=Response
3) wake client

*/

struct BcastCtrlData
{
    enum { Free=-1 };

    BcastCtrlData() : msgType(NoMsg), id(Free), generator(0) {}

    BcastCtrlRequest    request;
    BcastCtrlResponse   response;
    BcastCtrlMsgType    msgType;

    int                 id;
    int                 generator;

    int setId()                     { return id = generator++; }
    bool isFree() const             { return id==Free   && msgType==NoMsg; }
    bool isResponse(int id_) const  { return id==id_    && msgType==Response; }
    bool isRequest() const          { return id!=Free   && msgType==Request; }
    void free()                     { id=Free; msgType=NoMsg; }
};

typedef IPC::SynchroMem< BcastCtrlData, 1, 1> SharedBcastCtrlData;

} // ns IPCFetch

#endif // IPCFETCH_BCASTCTRLCOMMON_H
