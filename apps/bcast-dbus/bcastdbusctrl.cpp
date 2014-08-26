#include "bcastdbusctrl.h"
#include <QtCore/QThread>
#include "ipcfetch/broadcast.h"
#include "org.salvum.BroadcastAdp.h"

class Bcast : public IPCFetch::Broadcast
{
public:
    Bcast(const char *shmem, IFetch *fetch) : IPCFetch::Broadcast(shmem, fetch) 
    {
    }
};

class BcastThread : public QThread
{
public:
    BcastThread(Bcast &bcast) : m_bcast(bcast) 
    {
    }
protected:
    void run()
    {
	m_bcast.write();
    }
    Bcast &m_bcast;
};

class BcastDbusCtrl::Private 
{
public:
    Private(const char *shmem): bcast(shmem)

    Bcast	bcast;
    BcastThread thread;
};

///////////////////////////////////////////////

BcastDbusCtrl::BcastDbusCtrl(const char *shmem)
: m_d(new Private(shmem));
{
    new BroadcastAdaptor(this);
}

BcastDbusCtrl::~BcastDbusCtrl()
{
    delete m_d;
    m_d = nullptr;
}

void BcastDbusCtrl::start()
{
    if (!m_d.thread.isRunning()) {
	m_d.thread.start();
    }
}

void BcastDbusCtrl::skip(int clusterNo, int length)
{
}

void BcastDbusCtrl::stop()
{
    if (m_d.thread.isRunning()) {
	m_d.bcast.stop();
    }
}

void BcastDbusCtrl::emitProgress()
{
}
