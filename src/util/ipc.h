#ifndef IPC_H
#define IPC_H

#include "private/ipc_private.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <stdlib.h>
#include <unistd.h>

#include <type_traits>

namespace IPC {


class Mutex 
{
    friend class Cond;
public:
    Mutex();

    void init(bool pshared);
    void fini();

    void lock();
    void unlock();
    bool tryLock();

private:
    pthread_mutex_t mMtx;
};


class Cond
{
public:
    Cond();

    void init(bool pshared);
    void fini();

    void wait(Mutex &mtx);
    bool timedWait(Mutex &mtx, int msec); //< return true on signal, false on timeout
    void signal();
    void broadcast();
private:
    pthread_cond_t mCnd;
};



template <class T, int NMutexes=0, int NConds=0>
class SynchroMem: public T, public Mutexes<NMutexes>, public Conds<NConds>
{
//    static_assert( std::is_pod<T>::value, "SynchroMem<T>: T must be a POD type" );
    static_assert( !std::is_polymorphic<T>::value, "SynchroMem<T>: T must be a non-polymorphic type");
public:

    static SynchroMem<T, NMutexes, NConds> *create(const char *name);
    static SynchroMem<T, NMutexes, NConds> *attach(const char *name);

    static void destroy(SynchroMem<T, NMutexes, NConds> *mem, const char *name);
    static void detach(SynchroMem<T, NMutexes, NConds> *mem);

    Mutexes<NMutexes> &mutexes() { return *this; }
    Conds<NConds> &conds() { return *this; }
    
private:
    SynchroMem() {}
    ~SynchroMem() {}
};




} // ns IPC

#include "private/synchromem.cpp"

#endif // IPC_H
