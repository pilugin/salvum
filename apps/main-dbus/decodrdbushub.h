#ifndef DECODRDBUSHUB_H
#define DECODRDBUSHUB_H

#include "org.salvum.DecodrHubAdp.h"
#include "decoderdbuscontroller.h"
#include "models/workspacemodel.h"
#include <ui/qobjectlistmodel.h>
#include <ui/imageprovider.h>
#include <QPair>
#include <QMap>

class DecodrDbusHub : public Ui::QObjectListModel<DecoderDbusController>
                    , public Ui::ImageProviderAdaptor
{
    Q_OBJECT
public:
    DecodrDbusHub(QObject *parent =nullptr);
    ~DecodrDbusHub();

#if 0
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif
#endif

    void setWorkspaceModel(WorkspaceModel *wspace) { mWspace = wspace; }
    
    Q_INVOKABLE int getRewindCluster() const;
    
#if 0    
    QVariant data(const QModelIndex &index, int role) const;
#endif    
    QImage get(const QString &image) const;

public slots:
    QDBusObjectPath aquireClient(int clientId);
    void releaseClient(int clientId);   
    
    void createDecoders(const QString &shmemPath, const QList<int> &heads);
    void createDecoders(const QString &shmemPath, const QVariant &heads);
    
    void startProcessing();
    
signals:
    void allDecodersConnected();
    void allDecodersWaitForCheck();
    void allDecodersChecked();
    
private slots:
    void decodrConnected(bool connected);
    void decodingChanged(bool decoding);

private:    
    const DecoderDbusController *decoderByClientId(int clientId) const;

    WorkspaceModel *mWspace;
    QList<int> mHeads;
    QString mShmemPath;
};

#endif
