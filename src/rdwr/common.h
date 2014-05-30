#ifndef RDWR_COMMON_H
#define RDWR_COMMON_H

#include <util/ipc.h>

namespace RdWr {

template <class T>
struct ShMemGen //< type meta-generator. use ShMem<T>::SharedData
{
    struct Data
    {     
        T data;
        int regCount;
        int readCount;
    };

    typedef IPC::SynchroMem< Data, 2, 2> SharedData;
};


enum {
    REG = 0,    //< mutex<0> == "reg"
    READ = 1,   //< mutex<1> == "read"
    RD = 0,     //< cond<0> == "rd"
    WR = 1,     //< cond<1> == "wr"
};

} // ns RdWr

#endif