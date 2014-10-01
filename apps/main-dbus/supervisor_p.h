#ifndef SUPERVISOR_P_H
#define SUPERVISOR_P_H

#include "supervisor.h"
#include <QStateMachine>

class Supervisor::Private : public QObject
{
    friend class Supervisor;
    Q_OBJECT    
        
    Private(Supervisor *parent);
    void createFSM();
    
    Supervisor *owner;    
    QStateMachine fsm;
    
    QString qmlScreen;    
    QList<int> decodedHeads;
    
signals:
    void startDecode();    
    void allDecodersConnected();
    void allDecodersWaitForCheck();
    void broadcastAtEnd();
    void proceedDecode();    
    
};

#endif // SUPERVISOR_P_H
