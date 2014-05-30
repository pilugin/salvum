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
    virtual bool process(const T &data) =0;

private:
    Reader(); //< disable
    Reader(const Reader<T> &); //< disable
    Reader<T> &operator=(const Reader<T> &); //< disable

    typedef typename ShMemGen<T>::SharedData ShMem;

    ShMem *mMem;
};

} // ns RdWr

#include "private/reader.cpp"

#endif