#include "decoderdbuscontroller.h"
#include "org.salvum.DecodrCtrlAdp.h"
#include <jpeg/imagehelpers.h>
#include "models/customclustersmodel.h"
#include <QStateMachine>
#include <QTimer>

using namespace Ui;

class DecoderDbusController::Private
{
public:

    Private(int clientId_, const QDBusObjectPath &dbusObjectPath_, ImageProvider *imgProv, DecoderDbusController *owner_)
    : clientId(clientId_)
    , dbusObjectPath(dbusObjectPath_)
    , owner(owner_)
    , dbus(new DecodrCtrlAdaptor(owner_))
    , decodedClusters(new CustomClustersModel(imgProv, owner_))
    , heartbeatTimer(new QTimer(owner_))
    {
        heartbeatTimer->setSingleShot(true);
        heartbeatTimer->setInterval(15000);
        heartbeatTimer->start();        
        
        properties.cluster = properties.baselineCluster = properties.clustersDecoded = properties.blocksDecoded 
            = properties.blocksTotal = 0;
        properties.checked = properties.decoding = properties.decodingEnd = properties.decodingSuccess = false;

        QObject::connect(owner_, SIGNAL(decodedClustersUpdated(Common::DecodedClusters,Common::RejectedClusters,Common::ImageInfo)),
                         decodedClusters, SLOT(reset(Common::DecodedClusters,Common::RejectedClusters,Common::ImageInfo))      );
        QObject::connect(decodedClusters, SIGNAL(currentClusterParamsChanged(int,int,int)),
                         owner_, SLOT(clusterBaselined(int))  );
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
        QState *st2_checked = new QState( st1_connected );
        QState *st2_end = new QState( st1_connected );
        
        st1_connected->setInitialState( st2_init );
        
        st1_init        ->assignProperty(owner, "connected", false);
        st1_init        ->assignProperty(owner, "started", false);
        st1_connected   ->assignProperty(owner, "connected", true);
        st1_disconnected->assignProperty(owner, "connected", false);
        st2_decoding    ->assignProperty(owner, "decoding", true);
        st2_decoding    ->assignProperty(owner, "checked", false);
        st2_check       ->assignProperty(owner, "decoding", false);
        st2_end         ->assignProperty(owner, "decoding", false);
        st2_check       ->assignProperty(owner, "checked", false);
        st2_checked     ->assignProperty(owner, "checked", true);
        
        st1_init        ->addTransition(owner, SIGNAL(heartbeatRecv()), st1_connected);
        st1_connected   ->addTransition(heartbeatTimer, SIGNAL(timeout()), st1_disconnected);        
        st2_init        ->addTransition(owner, SIGNAL(start(int,QString,QString)), st2_started);
        st2_started     ->addTransition(owner, SIGNAL(progressChanged()), st2_decoding);
        st2_decoding    ->addTransition(owner, SIGNAL(decodingEndChanged()), st2_end);
        st2_decoding    ->addTransition(owner, 
                            SIGNAL(decodedClustersUpdated(Common::DecodedClusters,Common::RejectedClusters,Common::ImageInfo)),
                            st2_check);
        st2_check       ->addTransition(decodedClusters, SIGNAL(currentClusterChanged(int)), st2_checked);
        st2_checked     ->addTransition(owner, SIGNAL(progressChanged()), st2_decoding);
        
        fsm.setInitialState( st1_init );
    }

    const int clientId;
    const QDBusObjectPath dbusObjectPath;
    
    DecoderDbusController *owner;
    DecodrCtrlAdaptor *dbus;
    Ui::DecodedClustersModel *decodedClusters;
    QTimer *heartbeatTimer;

    QStateMachine fsm;    
    struct {
        bool connected;
        bool started;
        bool checked;
        bool decoding;
        int cluster;
        int baselineCluster;
        int clustersDecoded;
        int blocksDecoded;
        int blocksTotal;        
        bool decodingEnd;
        bool decodingSuccess;
        QString imagePath;
        QString thumbnailPath;
    } properties;
};

////////////////////////////////////////////

DecoderDbusController::DecoderDbusController(int clientId, const QDBusObjectPath &dbusObjectPath, ImageProvider *imgProv, QObject *parent)
: QObject(parent)
, m_d(new Private(clientId, dbusObjectPath, imgProv, this))
{
    m_d->createFSM();
    m_d->fsm.start();
}

DecoderDbusController::~DecoderDbusController()
{
    delete m_d;
}

int DecoderDbusController::clientId() const
{
    return m_d->clientId;
}

QDBusObjectPath DecoderDbusController::dbusObjectPath() const
{
    return m_d->dbusObjectPath;
}
    
bool DecoderDbusController::connected() const
{
    return m_d->properties.connected;
}

bool DecoderDbusController::started() const
{
    return m_d->properties.started;
}

bool DecoderDbusController::checked() const
{
    return m_d->properties.checked;
}

bool DecoderDbusController::decoding() const
{
    return m_d->properties.decoding;
}

int DecoderDbusController::cluster() const
{
    return m_d->properties.cluster;
}

int DecoderDbusController::baselineCluster() const
{
    return m_d->properties.checked ? m_d->properties.baselineCluster : 0;
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

Ui::DecodedClustersModel *DecoderDbusController::decodedClusters() const
{
    return m_d->decodedClusters;
}

QString DecoderDbusController::imagePath() const
{
    return m_d->properties.imagePath;
}

QString DecoderDbusController::thumbnailPath() const
{
    return m_d->properties.thumbnailPath;
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

void DecoderDbusController::setChecked(bool value)
{
    m_d->properties.checked = value;
    emit checkedChanged(value);
}

void DecoderDbusController::setDecoding(bool value)
{
    m_d->properties.decoding = value;
    emit decodingChanged(value);
}

// dbus

void DecoderDbusController::sendStart(int clusterNo, const QString &shmemPath, const QString &wspacePath)
{
    m_d->properties.cluster = clusterNo;
    emit clusterChanged(clusterNo);
    emit start(clusterNo, shmemPath, wspacePath);
}

void DecoderDbusController::sendBaseline()
{
    if (m_d->properties.checked)
        emit baseline(m_d->properties.baselineCluster);
    else
        qDebug()<<"Failed to sendBaseline, as Decoder is not CHECKED";
}

void DecoderDbusController::heartbeat()
{
//    qDebug()<<"<3";
    m_d->heartbeatTimer->start();
    emit heartbeatRecv();
}

void DecoderDbusController::decodingEnd(bool success)
{
    qDebug()<<cluster()<<__FUNCTION__<<success;

    m_d->properties.decodingEnd = true;
    m_d->properties.decodingSuccess = success;
    emit decodingEndChanged();
}

void DecoderDbusController::fetchAtEnd(bool complete, 
                    const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, 
                    const Common::ImageInfo &imageInfo)
{
    qDebug()<<cluster()<<__FUNCTION__<<imageInfo.imagePath;

    if (complete)
        decodingEnd(true);

    m_d->properties.imagePath = imageInfo.imagePath;
    emit imagePathChanged();
    emit decodedClustersUpdated(decodedClusters, rejectedClusters, imageInfo);
}
                    
void DecoderDbusController::progress(int clustersDecoded, int blocksDecoded, int blocksTotal)
{
    m_d->properties.clustersDecoded = clustersDecoded;
    m_d->properties.blocksDecoded = blocksDecoded;
    m_d->properties.blocksTotal = blocksTotal;
    
    emit progressChanged();
}

void DecoderDbusController::thumbnailCreated(const QString &path)
{
    qDebug()<<__FUNCTION__<<path;
    m_d->properties.thumbnailPath = path;
    emit thumbnailPathChanged();
}

// decodedClustersModel

void DecoderDbusController::clusterBaselined(int clusterNo)
{
    m_d->properties.baselineCluster = clusterNo;
    emit baselineClusterChanged(clusterNo);
}


