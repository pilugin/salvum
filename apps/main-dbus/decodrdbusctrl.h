#ifndef DECODRDBUSCTRL_H
#define DECODRDBUSCTRL_H

#include "org.salvum.DecodrCtrlAdp.h"

class QTimer;

class DecodrDbusCtrl : public QObject
{
    Q_OBJECT
    
public:
    DecodrDbusCtrl(QObject *parent =nullptr);
    ~DecodrDbusCtrl();
    
public slots:    
    void sendStart(int clusterNo)       { emit start(clusterNo); }
    void sendResume()                   { emit resume(); }
    void sendBaseline(int clusterNo)    { emit baseline(clusterNo); }
signals:
    void atEndRecv(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);
    void noHeartbeat();
    
private slots:
    void atEnd(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap)
                                        { emit atEndRecv(complete, decodedClusters, rejectedClusters, pixmap); }
    void heartbeat();
        
signals:    
    void start(int clusterNo);
    void resume();
    void baseline(int clusterNo);

private:
    QTimer *mHeartbeatTimer;
};

#endif
