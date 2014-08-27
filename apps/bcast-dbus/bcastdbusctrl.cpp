#include "bcastdbusctrl.h"
#include <QtCore/QThread>
#include "devicemapfetch.h"
#include "ipcfetch/broadcast.h"
#include "util/slotclosure.h"
#include "org.salvum.BroadcastAdp.h"

class Bcast : public IPCFetch::Broadcast
{
public:
    typedef IPCFetch::Broadcast Super;

    Bcast(const char *shmem, IFetch *fetch_, const SlotClosure &progressCallback) 
    : IPCFetch::Broadcast(shmem, fetch_) 
    , mProgressCallback(progressCallback)
    {
	mProgress.currentCluster = 0;
	mProgress.clusterCount = fetch_->bitmap().size();
    }
            
    void emitProgress()
    {
	interrupt(EmitProgress);
    }
    
    void skip(int clusterNo, int length)
    {
	mSkip.clusterNo = clusterNo;
	mSkip.length = length;
	interrupt(Skip);
    }
private:
    enum InternalOperation 
    {
	EmitProgress = Super::Custom,
	Skip,
	Custom
    };
    
    bool processInternalMsg(int internalMsg)
    {
	switch (internalMsg) {
	case EmitProgress:	
	    doEmitProgress();
	    return true;
	case Skip:
	    fetch()->skip(mSkip.clusterNo, mSkip.length);
	    return true;
	default:
	    return Super::processInternalMsg(internalMsg);
	}
    }
    
    void doEmitProgress()
    {
	qDebug("DO_EMIT_PROGRESS");
	mProgressCallback.call(Q_ARG(int, mProgress.currentCluster), Q_ARG(int, mProgress.clusterCount));
    }
    
    void postRead(const IPCFetch::BroadcastMessage &message)
    {
	if (message.status == IPCFetch::AtEnd)
	    mProgress.currentCluster = mProgress.clusterCount;
	else if (message.clusters.empty())
	    mProgress.currentCluster = 0;
	else
	    mProgress.currentCluster = message.clusters.back().clusterNo;
	    
	doEmitProgress();
	Super::postRead(message);
    }
    
    struct { int clusterNo, length; } mSkip;
    struct { int currentCluster, clusterCount; } mProgress;
    SlotClosure mProgressCallback;
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
    Private(const char *shmem, IFetch *fetch, const SlotClosure &progressCallback)
    : bcast(shmem, fetch, progressCallback)
    , thread(bcast)
    {
    }

    Bcast	bcast;
    BcastThread thread;
};

///////////////////////////////////////////////

BcastDbusCtrl::BcastDbusCtrl(const char *mediaPath, const char *bitmapPath)
: m_d(new Private(
	shmemPath(),
	new DeviceMapFetch(QString(mediaPath), QString(bitmapPath)),
	SlotClosure(this, SIGNAL(progress(int,int)))
    ))
{
    new BroadcastAdaptor(this);
    
    connect(&m_d->thread, SIGNAL(finished()), this, SIGNAL(stopped()) );
}

BcastDbusCtrl::~BcastDbusCtrl()
{
    delete m_d;
    m_d = nullptr;
}

void BcastDbusCtrl::start()
{
    if (!m_d->thread.isRunning()) {
	m_d->thread.start();
    }
}

void BcastDbusCtrl::skip(int clusterNo, int length)
{
    m_d->bcast.skip(clusterNo, length);
}

void BcastDbusCtrl::stop()
{
    if (m_d->thread.isRunning()) {
	m_d->bcast.stop();
    }
}

void BcastDbusCtrl::emitProgress()
{
    m_d->bcast.emitProgress();
}

bool BcastDbusCtrl::isValid() const
{
    return m_d->bcast.isValid();
}