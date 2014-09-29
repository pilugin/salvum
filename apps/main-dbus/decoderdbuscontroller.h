#ifndef DECODER_DBUS_CONTROLLER_H
#define DECODER_DBUS_CONTROLLER_H

#include <QtCore/QObject>
#include <common/types.h>

class DecoderDbusController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connected       READ connected  WRITE setConnected  NOTIFY connectedChanged)    //< set by FSM
    Q_PROPERTY(bool started         READ started    WRITE setStarted    NOTIFY startedChanged)      //< set by FSM
    Q_PROPERTY(int cluster          READ cluster                        NOTIFY clusterChanged)
    
    Q_PROPERTY(int clustersDecoded  READ clustersDecoded                NOTIFY progressChanged)
    Q_PROPERTY(int blocksDecoded    READ blocksDecoded                  NOTIFY progressChanged)
    Q_PROPERTY(int blocksTotal      READ blocksTotal                    NOTIFY progressChanged)
    
    Q_PROPERTY(bool decodingEnd     READ decodingEnd                    NOTIFY decodingEndChanged)
    Q_PROPERTY(bool decodingSuccess READ decodingSuccess                NOTIFY decodingEndChanged)
public:
    DecoderDbusController(QObject *parent =nullptr);
    ~DecoderDbusController();
    
    bool connected() const;
    bool started() const;
    int cluster() const;
    int clustersDecoded() const;
    int blocksDecoded() const;
    int blocksTotal() const;
    bool decodingEnd() const;
    bool decodingSuccess() const;
    
public slots:
    void sendStart(int clusterNo, const QString &shmemPath, const QString &wspacePath);    
    void sendBaseline(int clusterNo);
//    void exit();
           
private slots:
    void setConnected(bool value);
    void setStarted(bool value);

    void decodingEnd(bool success);
    void fetchAtEnd(bool complete, 
                    const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, 
                    const Common::Pixmap &pixmap);
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);
    void heartbeat();    
    
signals:    
    void connectedChanged(bool value);
    void startedChanged(bool value);
    void clusterChanged(int value);
    void progressChanged();   
    void decodingEndChanged();
    void decodedClustersUpdated(const Common::DecodedClusters &decodedClusters, 
                                const Common::RejectedClusters &rejectedClusters, 
                                const Common::Pixmap &pixmap);    
    void start(int clusterNo, const QString &shmemPath, const QString &wspacePath);
    void baseline(int clusterNo);
    
private:
    class Private;
    Private *m_d;    
};

#endif // DECODER_DBUS_CONTROLLER_H