#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <type_traits>

/// very simple array class for IPC exchange

template <class T, int N>
class Array
{
//    static_assert(std::is_pod<T>::value, "Array<T>: T must be a POD type");
public:
    Array() : mSize(0) {}

    static int capacity() { return N; }

    int size() const { return mSize; }
    bool full() const { return mSize == N; }
    void clear() { mSize=0; }
    void push_back(const T &item)
    {
        assert(mSize < N);
        mData[mSize++] = item;
    }    
    void push_back_empty()
    {
        assert(mSize < N);
        ++mSize;
    }
    T &back() 
    { 
        assert(mSize > 0);
        return mData[mSize -1];
    }
    const T &back() const
    {
        assert(mSize > 0);
        return mData[mSize -1];
    }
    T pop_back()
    {
        assert(mSize>0);
        return mData[mSize--];
    }
    const T &operator[](int index) const
    {
        assert(index>=0 && index<mSize);
        return mData[index];
    }
    T &operator[](int index)
    {
        assert(index>=0 && index<mSize);
        return mData[index];
    }
    T *data() { return mData; }
    const T *data() const { return mData; }
    void set(const T *data, int size)
    {
        assert(size <= N);
        mSize = size;
        memcpy(mData, data, size * sizeof(T) );
    }

private:
    int mSize;
    T mData[N];
};

#endif
