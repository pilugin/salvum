#include "supervisor.h"
#include <QtDebug>

#include "supervisor_p.h"

Supervisor::Private::Private(Supervisor *owner_)
: QObject(owner_)
, owner(owner_)
{    
    createFSM();
}

void Supervisor::Private::createFSM()
{
    QState *st1_setup = new QState(&fsm);
    QState *st1_prepareDecoders = new QState(&fsm);
    QState *st1_decode = new QState(&fsm);
    QState *st1_check = new QState(&fsm);
    
    st1_setup           ->assignProperty(owner, "qmlScreen", "Setup.qml");
    st1_prepareDecoders ->assignProperty(owner, "qmlScreen", "Decode.qml");
    st1_decode          ->assignProperty(owner, "qmlScreen", "Decode.qml");
    st1_check           ->assignProperty(owner, "qmlScreen", "Check.qml");
        
    st1_setup           ->addTransition(this, SIGNAL(startDecode()),            st1_prepareDecoders);
    st1_prepareDecoders ->addTransition(this, SIGNAL(allDecodersConnected()),   st1_decode);
//    st1_decode          ->addTransition(this, SIGNAL(broadcastAtEnd()),         st1_check);
    st1_decode          ->addTransition(this, SIGNAL(allDecodersWaitForCheck()), st1_check);
    st1_check           ->addTransition(this, SIGNAL(proceedDecode()),          st1_decode);

    connect(st1_prepareDecoders,SIGNAL(entered()),  owner, SIGNAL(prepareDecodersStateEntered())    );
    connect(st1_prepareDecoders,SIGNAL(exited()),   owner, SIGNAL(prepareDecodersStateExited())     );
    
    fsm.setInitialState(st1_setup);
}



Supervisor::Supervisor(QObject *parent)
: QObject(parent)
{
    m_d = new Private(this);
}

void Supervisor::startDecode(const QList<int> &clusters)
{
    m_d->decodedHeads = clusters;
    emit decodedHeadsChanged();
    emit m_d->startDecode();
}

void Supervisor::startDecode(const QVariant &clusters)
{
    QList<int> c;
    bool ok;
    for (QVariant cluster : clusters.toList()) {
        int intc = cluster.toInt(&ok);
        if (ok)
            c.push_back(intc);
        else
            qDebug()<<"ERR: Supervisor::startDecode. failed to cast to int " << clusters;
    }
    startDecode(c);
}

QString Supervisor::qmlScreen() const
{
    return m_d->qmlScreen;
}

void Supervisor::qmlScreen(const QString &value) 
{
    if (m_d->qmlScreen != value) {
        m_d->qmlScreen = value;
        emit qmlScreenChanged();
    }
}

QList<int> Supervisor::decodedHeads() const
{
    return m_d->decodedHeads;
}

void Supervisor::allDecodersConnected()
{
    emit m_d->allDecodersConnected();
}

void Supervisor::allDecodersWaitForCheck()
{
    emit m_d->allDecodersWaitForCheck();
}

void Supervisor::proceedDecode()
{
    emit m_d->proceedDecode();
}

void Supervisor::broadcastAtEnd()
{
    emit m_d->broadcastAtEnd();
}

void Supervisor::init()
{
    m_d->fsm.start();
}

