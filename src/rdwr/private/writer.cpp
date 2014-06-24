// included from ../writer.h to hide implementation of the template



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

    // internals
    mInternalMtx.init(false);
    mInternalCnd.init(false);
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

    bool cont = true;
    while (cont) {

        cont = cont && prepare(mMem->data);

        // wait for at least one reader to register
        m.mutex<REG>().lock();

        while ( mMem->regCount == 0 && cont) {
            c.cond<WR>().wait( m.mutex<REG>() );
            cont = cont && checkInternalMsg();
        }

        // wake all readers
        c.cond<RD>().broadcast();
        mMem->readCount = 0;
        while ( mMem->readCount < mMem->regCount )  {
            // here we open both mutexes, so client can READ or UNREGISTER
            m.mutex<REG>().unlock();
            c.cond<WR>().wait( m.mutex<READ>() );
            m.mutex<REG>().lock();

            cont = cont && checkInternalMsg();
        }

        postRead(mMem->data);
        
        m.mutex<REG>().unlock();

    } // loop


    // wait until all readers unregister
    if ( mMem->regCount > 0) {
        cont = true;    
        m.mutex<REG>().lock();
        while ( mMem->regCount > 0 && cont) {
            if ( ! c.cond<WR>().timedWait( m.mutex<REG>(), noregTimeout ) ) {
//                mMem->regCount = 0;
                break;
            }

            cont = checkInternalMsg();
        }
        m.mutex<REG>().unlock();
    }
}

template <class T>
void Writer<T>::postRead(const T &)
{
}

//////////////////// internal control

template <class T>
void Writer<T>::stop()
{
    interrupt(Exit);
}

template <class T>
void Writer<T>::pause()
{
    interrupt(Pause);  
}

template <class T>
void Writer<T>::resume()
{
    mInternalCnd.signal();
}

template <class T>
void Writer<T>::interrupt(int msg)
{
    IPC::Conds<2> &c = mMem->conds();
    static const int to = 1*1000;

    mInternalMtx.lock();
    mInternalMsg = msg;

    if (msg == Exit )
        mInternalCnd.signal();

    c.cond<WR>().signal();
    while ( ! mInternalCnd.timedWait( mInternalMtx, to ) ) 
        c.cond<WR>().signal();

    mInternalMtx.unlock();
}

template <class T>
bool Writer<T>::checkInternalMsg()
{
    bool rv = true;
    if ( mInternalMsg != Noop ) {
        mInternalMtx.lock();

        rv = processInternalMsg(mInternalMsg);
        mInternalMsg = Noop;
        
        mInternalCnd.signal();
        mInternalMtx.unlock();
    }
    return rv;
}

template <class T>
bool Writer<T>::processInternalMsg(int internalMsg)
{
    switch (internalMsg) {
    case Pause:
        mInternalCnd.signal();
        mInternalCnd.wait( mInternalMtx );
        return mInternalMsg != Exit;

    case Exit:
        return false;
    default:
        break;
    }
    return true;
}


} // ns RdWr
