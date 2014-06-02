#ifndef READER_H
#define READER_H

#include <rdwr/common.h>

namespace RdWr {

template <class T>
class Reader
{
public:

    Reader(const char *shmemName);
    virtual ~Reader();

    virtual bool isValid() const { return mMem != nullptr; }     

    void reg();
    void read();
    void unreg();
    
protected:
    virtual void duringReg() {}
    virtual bool process(const T &data) =0;
    virtual void duringUnreg() {}

    typedef typename ShMemGen<T>::SharedData ShMem;
    const ShMem &sharedMem() const { return *mMem; }
    T &sharedData() { return mMem->data; }
    const T &sharedData() const { return mMem->data; }


private:
    Reader(); //< disable
    Reader(const Reader<T> &); //< disable
    Reader<T> &operator=(const Reader<T> &); //< disable

    ShMem *mMem;
};

} // ns RdWr

#include "private/reader.cpp"

#endif