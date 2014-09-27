#ifndef DECODRDBUSCTRL_H
#define DECODRDBUSCTRL_H

#include "org.salvum.DecodrCtrlAdp.h"

class QTimer;

class DecodrDbusCtrl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected     READ isConnected    NOTIFY statusChanged)
    Q_PROPERTY(bool isStarted       READ isStarted      NOTIFY statusChanged)
    Q_PROPERTY(int cluster          READ cluster        NOTIFY statusChanged)
    Q_PROPERTY(QString status       READ status         NOTIFY statusChanged)
    
    Q_PROPERTY(int clustersDecoded  READ clustersDecoded    NOTIFY progressChanged)
    Q_PROPERTY(int blocksDecoded    READ blocksDecoded      NOTIFY progressChanged)
    Q_PROPERTY(int blocksTotal      READ blocksTotal        NOTIFY progressChanged)
    
    
public:
    DecodrDbusCtrl(QObject *parent =nullptr);
    ~DecodrDbusCtrl();
    
    bool isConnected() const    { return mStatus>=Connected && mStatus<Disconnected; }
    bool isStarted() const      { return mStatus>=Started && mStatus<Disconnected; }
    int cluster() const         { return mCluster; }
    int clustersDecoded() const { return mClustersDecoded; }
    int blocksDecoded() const   { return mBlocksDecoded; }
    int blocksTotal() const     { return mBlocksTotal; }    
    QString status() const;
    
public slots:    
    void sendStart(int clusterNo, const QString &shmemPath);
    void sendBaseline(int clusterNo)    { emit baseline(clusterNo); }
signals:
    void fetchAtEndRecv(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);                                         
    void noHeartbeat();    
    void progressChanged();
    void statusChanged();
    
    void connected();
    void disconnected();
    
private slots:
    void fetchAtEnd(bool complete, const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap)
                                        { emit fetchAtEndRecv(complete, decodedClusters, rejectedClusters, pixmap); }
    void decodingEnd(bool success);
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);                                        
    void heartbeat();
    
    void onNoHeartbeat();
        
signals:    
    void start(int cluterNo, const QString &shmemPath);
    void baseline(int clusterNo);

private:
    QTimer *mHeartbeatTimer;
    int mCluster;
    int mClustersDecoded;
    int mBlocksDecoded;
    int mBlocksTotal;
    
    enum {
        Initial,
        Connected,
        Started,
        Decoding,
        Disconnected,
        DecodingFailed,
        DecodingSuccess
    };
    int mStatus;
};

#endif
