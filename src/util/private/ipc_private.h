#ifndef IPC_PRIVATE_H
#define IPC_PRIVATE_H

#include <pthread.h>
#include "container.h"

namespace IPC {

class Cond;

class Mutex;

template <int N>
class Mutexes : public Container<Mutex, N>
{
public:
    template <int M =0>
    Mutex &mutex() { return GetHelper<Mutex, N, M>::get(this); }
};


template <int N>
class Conds : public Container<Cond, N>
{
public:
    template <int M =0>
    Cond &cond() { return GetHelper<Cond, N, M>::get(this); }
};




} // ns IPC

#endif