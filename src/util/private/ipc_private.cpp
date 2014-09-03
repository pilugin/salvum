#include "util/ipc.h"
#include <cerrno>
#include <cstring>
#include <QtDebug>

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
    if (int err = pthread_mutex_lock( &mMtx ))
        qDebug()<<__FUNCTION__<<strerror(err);
}

void Mutex::unlock()
{
    if (int err = pthread_mutex_unlock( &mMtx ))
        qDebug()<<__FUNCTION__<<strerror(err);
}

bool Mutex::tryLock()
{
    int err = pthread_mutex_unlock( &mMtx );
    if (err != 0 && err != EBUSY)
        qDebug()<<__FUNCTION__<<strerror(err);
    return err == 0;
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
    if (int err = pthread_cond_destroy( &mCnd ))
        qDebug()<<__FUNCTION__<<strerror(err);
}

void Cond::wait(Mutex &mtx)
{
    if (int err = pthread_cond_wait(&mCnd, &mtx.mMtx))
        qDebug()<<__FUNCTION__<<strerror(err);
}

bool Cond::timedWait(Mutex &mtx, int msec)
{
    int sec = msec / 1000;
    int nsec = (msec % 1000) * 1000*1000;
    struct timespec ts { time(nullptr) + sec, nsec };
    int err = pthread_cond_timedwait( &mCnd, &mtx.mMtx, &ts );
    if (err != ETIMEDOUT && err != 0)
        qDebug()<<__FUNCTION__<<strerror(err);
    return err != ETIMEDOUT;
}

void Cond::signal()
{
    if (int err = pthread_cond_signal(&mCnd))
        qDebug()<<__FUNCTION__<<strerror(err);
}

void Cond::broadcast()
{
    if (int err = pthread_cond_broadcast(&mCnd))
        qDebug()<<__FUNCTION__<<strerror(err);
}


} // ns IPC