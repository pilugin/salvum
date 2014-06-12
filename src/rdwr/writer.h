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

protected:

    virtual bool prepare(T &data) =0; //< Mutexes: REG=unlocked, READ=locked
    virtual void postRead(const T &data); //< empty method. called after all registered clients read. Mutexes: REG=locked, READ=locked

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
};

} // ns RdWr

#include "private/writer.cpp"

#endif
