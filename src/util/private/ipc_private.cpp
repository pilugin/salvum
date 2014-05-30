#include "../ipc.h"
#include <cerrno>

namespace IPC {

Mutex::Mutex()
{
}

void Mutex::init(bool pshared)
{
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init( &mutexattr );
    pthread_mutexattr_setpshared( &mutexattr, pshared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE );
    pthread_mutex_init( &mMtx, &mutexattr );
    pthread_mutexattr_destroy( &mutexattr );
}

void Mutex::fini()
{
    pthread_mutex_destroy( &mMtx );
}

void Mutex::lock()
{
    pthread_mutex_lock( &mMtx );
}

void Mutex::unlock()
{
    pthread_mutex_unlock( &mMtx );
}

//////

Cond::Cond()
{
}

void Cond::init(bool pshared)
{
    pthread_condattr_t condattr;
    pthread_condattr_init( &condattr );
    pthread_condattr_setpshared( &condattr, pshared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE );
    pthread_cond_init( &mCnd, &condattr );
    pthread_condattr_destroy( &condattr );
}

void Cond::fini()
{
    pthread_cond_destroy( &mCnd );
}

void Cond::wait(Mutex &mtx)
{
    pthread_cond_wait(&mCnd, &mtx.mMtx);
}

bool Cond::timedWait(Mutex &mtx, int msec)
{
    int sec = msec / 1000;
    int nsec = (msec % 1000) * 1000*1000;
    struct timespec ts { time(nullptr) + sec, nsec };
    return pthread_cond_timedwait( &mCnd, &mtx.mMtx, &ts ) != ETIMEDOUT;
}

void Cond::signal()
{
    pthread_cond_signal(&mCnd);
}

void Cond::broadcast()
{
    pthread_cond_broadcast(&mCnd);
}


} // ns IPC