#include "decoderdbuscontroller.h"
#include "org.salvum.DecodrCtrlAdp.h"
#include <QStateMachine>
#include <QTimer>

class DecoderDbusController::Private
{
public:

    Private(DecoderDbusController *owner_)
    : owner(owner_)
    , dbus(new DecodrCtrlAdaptor(owner_))
    , heartbeatTimer(new QTimer(owner_))
    {
        heartbeatTimer->setSingleShot(true);
        heartbeatTimer->setInterval(5000);
        heartbeatTimer->start();        
        
        properties.cluster = properties.clustersDecoded = properties.blocksDecoded = properties.blocksTotal = 0;
        properties.decodingEnd = properties.decodingSuccess = false;
    }

    void createFSM()
    {
        QState *st1_init = new QState( &fsm );
        QState *st1_connected = new QState( &fsm );
        QState *st1_disconnected = new QState( &fsm );
        
        QState *st2_init = new QState( st1_connected );
        QState *st2_started = new QState( st1_connected );
        QState *st2_decoding = new QState( st1_connected );
        QState *st2_check = new QState( st1_connected );
        QState *st2_end = new QState( st1_connected );
        
        st1_connected->setInitialState( st2_init );
        
        st1_init        ->assignProperty(owner, "connected", false);
        st1_init        ->assignProperty(owner, "started", false);
        st1_connected   ->assignProperty(owner, "connected", true);
        st1_disconnected->assignProperty(owner, "connected", false);
        
        st1_init        ->addTransition(dbus, SIGNAL(heartbeat()), st1_connected);
        st1_connected   ->addTransition(heartbeatTimer, SIGNAL(timeout()), st1_disconnected);        
        st2_init        ->addTransition(owner, SIGNAL(start(int,QString,QString)), st2_started);
        st2_started     ->addTransition(owner, SIGNAL(progressChanged()), st2_decoding);
        st2_decoding    ->addTransition(owner, SIGNAL(decodingEndChanged()), st2_end);
        st2_decoding    ->addTransition(owner, 
                            SIGNAL(decodedClustersChanged(Common::DecodedClusters,Common::RejectedClusters,Common::Pixmap)), 
                            st2_check);
        st2_check       ->addTransition(owner, SIGNAL(baseline(int)), st2_decoding);
        
        fsm.setInitialState( st1_init );
        fsm.start();
    }

    DecoderDbusController *owner;
    DecodrCtrlAdaptor *dbus;
    QTimer *heartbeatTimer;

    QStateMachine fsm;    
    struct {
        bool connected;
        bool started;
        int cluster;
        int clustersDecoded;
        int blocksDecoded;
        int blocksTotal;        
        bool decodingEnd;
        bool decodingSuccess;
    } properties;
};

////////////////////////////////////////////

DecoderDbusController::DecoderDbusController(QObject *parent)
: QObject(parent)
, m_d(new Private(this))
{
}

DecoderDbusController::~DecoderDbusController()
{
    delete m_d;
}
    
bool DecoderDbusController::connected() const
{
    return m_d->properties.connected;
}

bool DecoderDbusController::started() const
{
    return m_d->properties.started;
}

int DecoderDbusController::cluster() const
{
    return m_d->properties.cluster;
}

int DecoderDbusController::clustersDecoded() const
{
    return m_d->properties.clustersDecoded;
}

int DecoderDbusController::blocksDecoded() const
{
    return m_d->properties.blocksDecoded;
}

int DecoderDbusController::blocksTotal() const
{
    return m_d->properties.blocksTotal;
}

bool DecoderDbusController::decodingEnd() const
{
    return m_d->properties.decodingEnd;
}

bool DecoderDbusController::decodingSuccess() const
{
    return m_d->properties.decodingSuccess;
}
    
void DecoderDbusController::setConnected(bool value)
{
    m_d->properties.connected = value;
    emit connectedChanged(value);
}

void DecoderDbusController::setStarted(bool value)
{
    m_d->properties.started = value;
    emit startedChanged(value);
}

// dbus

void DecoderDbusController::sendStart(int clusterNo, const QString &shmemPath, const QString &wspacePath)
{
    m_d->properties.cluster = clusterNo;
    emit clusterChanged(clusterNo);
    emit start(clusterNo, shmemPath, wspacePath);
}

void DecoderDbusController::sendBaseline(int clusterNo)
{
    emit baseline(clusterNo);
}

void DecoderDbusController::heartbeat()
{
    m_d->heartbeatTimer->start();
}

void DecoderDbusController::decodingEnd(bool success)
{
    m_d->properties.decodingEnd = true;
    m_d->properties.decodingSuccess = success;
    emit decodingEndChanged();
}

void DecoderDbusController::fetchAtEnd(bool complete, 
                    const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, 
                    const Common::Pixmap &pixmap)
{
    if (complete)
        decodingEnd(true);

    emit decodedClustersUpdated(decodedClusters, rejectedClusters, pixmap);
}
                    
void DecoderDbusController::progress(int clustersDecoded, int blocksDecoded, int blocksTotal)
{
    m_d->properties.clustersDecoded = clustersDecoded;
    m_d->properties.blocksDecoded = blocksDecoded;
    m_d->properties.blocksTotal = blocksTotal;
    
    emit progressChanged();
}

