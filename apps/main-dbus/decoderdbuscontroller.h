#ifndef DECODER_DBUS_CONTROLLER_H
#define DECODER_DBUS_CONTROLLER_H

#include <QtCore/QObject>
#include <QtDBus/QDBusObjectPath>
#include <common/types.h>
#include <ui/decodedclustersmodel.h>

class DecoderDbusController : public QObject
{
    Q_OBJECT

    // state
    Q_PROPERTY(bool connected       READ connected  WRITE setConnected  NOTIFY connectedChanged)    //< set by FSM
    Q_PROPERTY(bool started         READ started    WRITE setStarted    NOTIFY startedChanged)      //< set by FSM
    Q_PROPERTY(bool checked         READ checked    WRITE setChecked    NOTIFY checkedChanged)      //< set by FSM
    Q_PROPERTY(bool decoding        READ decoding   WRITE setDecoding   NOTIFY decodingChanged)     //< set by FSM

    Q_PROPERTY(int cluster          READ cluster                        NOTIFY clusterChanged)
    Q_PROPERTY(int baselineCluster  READ baselineCluster                NOTIFY baselineClusterChanged)

    Q_PROPERTY(QObject* decodedClusters
                                    READ decodedClusters                CONSTANT)
    
    // progress
    Q_PROPERTY(int clustersDecoded  READ clustersDecoded                NOTIFY progressChanged)
    Q_PROPERTY(int blocksDecoded    READ blocksDecoded                  NOTIFY progressChanged)
    Q_PROPERTY(int blocksTotal      READ blocksTotal                    NOTIFY progressChanged)
    Q_PROPERTY(QString imagePath    READ imagePath                      NOTIFY imagePathChanged)
    
    // after decoding
    Q_PROPERTY(bool decodingEnd     READ decodingEnd                    NOTIFY decodingEndChanged)
    Q_PROPERTY(bool decodingSuccess READ decodingSuccess                NOTIFY decodingEndChanged)
public:
    DecoderDbusController(int clientId, const QDBusObjectPath &dbusObjectPath, Ui::ImageProvider *imgProv, QObject *parent =nullptr);
    ~DecoderDbusController();
    
    int clientId() const;
    QDBusObjectPath dbusObjectPath() const;
    bool connected() const;
    bool started() const;
    bool checked() const;
    bool decoding() const;
    int cluster() const;
    int baselineCluster() const;
    int clustersDecoded() const;
    int blocksDecoded() const;
    int blocksTotal() const;
    bool decodingEnd() const;
    bool decodingSuccess() const;

    Ui::DecodedClustersModel *decodedClusters() const;
    QString imagePath() const;
    
public slots:
    void sendStart(int clusterNo, const QString &shmemPath, const QString &wspacePath);    
    void sendBaseline();
//    void exit();
           
private slots:
    void setConnected(bool value);
    void setStarted(bool value);
    void setChecked(bool value);
    void setDecoding(bool value);

    void decodingEnd(bool success);
    void fetchAtEnd(bool complete, 
                    const Common::DecodedClusters &decodedClusters, 
                    const Common::RejectedClusters &rejectedClusters, 
                    const Common::ImageInfo &imageInfo);
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);
    void heartbeat();    
    
    // decodedClustersModel
    void clusterBaselined(int clusterNo);

signals:    
    void connectedChanged(bool value);
    void startedChanged(bool value);
    void checkedChanged(bool value);
    void decodingChanged(bool value);
    void clusterChanged(int value);    
    void baselineClusterChanged(int value);
    void progressChanged();   
    void imagePathChanged();
    void decodingEndChanged();
    void decodedClustersUpdated(const Common::DecodedClusters &decodedClusters, 
                                const Common::RejectedClusters &rejectedClusters, 
                                const Common::ImageInfo &imageInfo);

    void start(int clusterNo, const QString &shmemPath, const QString &wspacePath);
    void baseline(int clusterNo);
    
    void heartbeatRecv();
private:
    class Private;
    Private *m_d;    
};

#endif // DECODER_DBUS_CONTROLLER_H
