#ifndef CONTAINER_H
#define CONTAINER_H

namespace IPC {


template <class T, int N> class Container;


template <class T, int N> 
class InitHelper
{
public:
    template <class Y>
    static void init( Container<T, N> *c, Y param ) 
    { 
        c->head().init(param);
        InitHelper<T, N-1>::init( c, param );
    }
};

template <class T> class InitHelper<T, 0>
{
public:
    template <class Y>
    static void init( Container<T, 0> *, Y ) {}
};



template <class T, int N>
class FiniHelper
{
public:
    static void fini( Container<T, N> *c )
    {
        c->head().fini();
        FiniHelper<T, N-1>::fini( c );
    }
};

template <class T> class FiniHelper<T, 0>
{
public:
    static void fini( Container<T, 0> * ) {}
};



template <class T, int N, int M> 
class GetHelper
{
public:
    static T &get( Container<T, N> *c ) { return GetHelper<T, N-1, M-1>::get(c); }
};

template <class T, int N> class GetHelper<T, N, 0>
{
public:
    static T &get( Container<T, N> *c ) { return c->head(); }
};





template <class T, int N>
class Container : public Container<T, N-1>
{
public:
    template <int M>
    T &get() { return GetHelper<T, N, M>::get(this); } 
    
    template <class Y>
    void init(Y param) { InitHelper<T, N>::init(this, param); }

    void fini() { FiniHelper<T, N>::fini(this); }

    T &head() { return mItem; }
private:
    T mItem;
};

template <class T>
class Container<T, 0>
{
public:
    template <class Y> void init(Y) {}
};

} // ns IPC

#endif