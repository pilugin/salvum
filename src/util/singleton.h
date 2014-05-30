#ifndef SINGLETON_H
#define SINGLETON_H

#include <QtGlobal>

template <class T>
class Singleton
{
public:
    static T *instance()
    {
        return sInstance;
    }

protected:
    Singleton() 
    {
        Q_ASSERT(!sInstance);
        sInstance = static_cast<T *>(this);
    }
    virtual ~Singleton()
    {
        Q_ASSERT(sInstance);
        sInstance = nullptr;
    }

private:
    static T *sInstance;
};

template <class T> T *Singleton<T>::sInstance = nullptr;

#endif
