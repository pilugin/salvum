// included from ../writer.h to hide implementation of the template



//#include <QtDebug>

namespace RdWr {

template <class T>
Writer<T>::Writer(const char *name)
: mMem( ShMem::create(name) )
, mShMemName(name)
{
    IPC::Mutexes<2> &m = mMem->mutexes(); //< workaround for gcc

    m.mutex<REG>().lock();
    mMem->regCount = 0;
    m.mutex<REG>().unlock();

    m.mutex<READ>().lock();
    mMem->readCount = 0;

}

template <class T>
Writer<T>::~Writer()
{
    if (mMem) {
        mMem->IPC::Mutexes<2>::mutex<READ>().unlock();

        ShMem::destroy(mMem, mShMemName.c_str());
    }
}

template <class T>
void Writer<T>::write()
{
    static const int noregTimeout = 20*60*1000;

    IPC::Mutexes<2> &m = mMem->mutexes(); //< workaround for gcc
    IPC::Conds<2> &c = mMem->conds();

    bool exit = false;
    for (;;) {

        if (!exit) {
            exit = ! prepare(mMem->data);

            // wait for at least one reader to register
            m.mutex<REG>().lock();
            while ( mMem->regCount == 0) {
                if ( ! c.cond<WR>().timedWait( m.mutex<REG>(), noregTimeout ) ) { //<timeout?
                    m.mutex<REG>().unlock();
                    return;
                }
            }
        
            // wake all readers
            c.cond<RD>().broadcast();
        
            mMem->readCount = 0;
            while ( mMem->readCount < mMem->regCount )  {
                // here we open both mutexes, so client can READ or UNREGISTER
                m.mutex<REG>().unlock();
                c.cond<WR>().wait( m.mutex<READ>() );
                m.mutex<REG>().lock();
            }

            postRead(mMem->data);
        
            m.mutex<REG>().unlock();

        } else {
            
            // wait until all readers unregister
            m.mutex<REG>().lock();
            while ( mMem->regCount > 0 )
                if ( ! c.cond<WR>().timedWait( m.mutex<REG>(), noregTimeout ) ) {
                    mMem->regCount = 0;
                    break;
                }
            m.mutex<REG>().unlock();

            break;
        }

    } // for
}

template <class T>
void Writer<T>::postRead(const T &)
{
}

} // ns RdWr
