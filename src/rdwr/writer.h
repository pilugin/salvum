#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <rdwr/common.h>

namespace RdWr {

template <class T>
class Writer
{
public:

    Writer(const char *shmemName);
    virtual ~Writer();

    virtual bool isValid() const { return mMem != nullptr; }

    virtual void write();

    void stop();
    void pause();
    void resume();

protected:
    enum InternalOperation { Noop, Pause, Resume, Exit, Custom };

    virtual bool prepare(T &data) =0; //< Mutexes: REG=unlocked, READ=locked
    virtual void postRead(const T &data); //< empty method. called after all registered clients read. Mutexes: REG=locked, READ=locked

    void interrupt(int msg=Custom);
    bool checkInternalMsg();
    virtual bool processInternalMsg();

    typedef typename ShMemGen<T>::SharedData ShMem;
    const ShMem &sharedMem() const { return *mMem; }
    T &sharedData() { return mMem->data; }
    const T &sharedData() const { return mMem->data; }
private:
    Writer(); //< disable
    Writer(const Writer<T> &); //< disable
    Writer<T> &operator=(const Writer<T> &); //< disable


    ShMem *mMem;
    std::string mShMemName;    

    IPC::Mutex  mInternalMtx;
    IPC::Cond   mInternalCnd;
    int         mInternalMsg;

};

} // ns RdWr

#include "private/writer.cpp"

#endif
