// included from ../reader.h to hide implementation of the template



//#include <QtDebug>

namespace RdWr {

template <class T>
Reader<T>::Reader(const char *name) 
: mMem( ShMem::attach(name) )
{
}

template <class T>
Reader<T>::~Reader()
{
    if (mMem)
        ShMem::detach(mMem);
}

template <class T>
void Reader<T>::reg()
{
    IPC::Mutexes<2> &m = mMem->mutexes(); //< workaround for gcc
    IPC::Conds<2> &c = mMem->conds();
    
    // register reader
    m.mutex<REG>().lock();
    ++ mMem->regCount;
//    qDebug()<<"REGISTERED"<<mMem->regCount;
    duringReg();
    m.mutex<REG>().unlock();
    c.cond<WR>().signal();
}

template <class T>
void Reader<T>::read()
{
    IPC::Mutexes<2> &m = mMem->mutexes(); //< workaround for gcc
    IPC::Conds<2> &c = mMem->conds();
    
    // start reading
    m.mutex<READ>().lock();

    bool exit = false;
    for (;;) {
        exit = ! process( mMem->data );
        ++ mMem->readCount;
        c.cond<WR>().signal();

        if (exit)
            break;

        c.cond<RD>().wait( m.mutex<READ>() );
    }

    m.mutex<READ>().unlock();
}

template <class T>
void Reader<T>::unreg()
{
    IPC::Mutexes<2> &m = mMem->mutexes(); //< workaround for gcc
    IPC::Conds<2> &c = mMem->conds();

//    qDebug("READER unregistering");

    // unregister reader
    m.mutex<REG>().lock();
    -- mMem->regCount;
    duringUnreg();
    m.mutex<REG>().unlock();
    c.cond<WR>().signal();

//    qDebug("READER unregistered");

}



} // ns RdWr
