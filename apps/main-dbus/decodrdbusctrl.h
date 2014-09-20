#ifndef DECODRDBUSCTRL_H
#define DECODRDBUSCTRL_H

#include "org.salvum.DecodrCtrlAdp.h"

class QTimer;

class DecodrDbusCtrl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected     READ isConnected    NOTIFY isConnectedChanged)
    Q_PROPERTY(bool isStarted       READ isStarted      NOTIFY isStartedChanged)
    Q_PROPERTY(int cluster          READ cluster        NOTIFY isStartedChanged)
    Q_PROPERTY(int clustersDecoded  READ clustersDecoded NOTIFY progressChanged)
    Q_PROPERTY(int blocksDecoded    READ blocksDecoded NOTIFY progressChanged)
    Q_PROPERTY(int blocksTotal      READ blocksTotal NOTIFY progressChanged)
public:
    DecodrDbusCtrl(QObject *parent =nullptr);
    ~DecodrDbusCtrl();
    
    bool isConnected() const    { return mIsConnected; }
    bool isStarted() const      { return mIsStarted; }
    int cluster() const         { return mCluster; }
    int clustersDecoded() const { return mClustersDecoded; }
    int blocksDecoded() const   { return mBlocksDecoded; }
    int blocksTotal() const     { return mBlocksTotal; }
    
public slots:    
    void sendStart(int clusterNo);
    void sendResume()                   { emit resume(); }
    void sendBaseline(int clusterNo)    { emit baseline(clusterNo); }
signals:
    void atEndRecv(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);                    
    void noHeartbeat();
    
    void isConnectedChanged();
    void isStartedChanged();
    void progressChanged();
    
private slots:
    void atEnd(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap)
                                        { emit atEndRecv(complete, decodedClusters, rejectedClusters, pixmap); }
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);                                        
    void heartbeat();
        
signals:    
    void start(int clusterNo);
    void resume();
    void baseline(int clusterNo);

private:
    QTimer *mHeartbeatTimer;
    bool mIsConnected;
    bool mIsStarted;
    int mCluster;
    int mClustersDecoded;
    int mBlocksDecoded;
    int mBlocksTotal;
};

#endif
