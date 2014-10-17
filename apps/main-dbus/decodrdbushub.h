#ifndef DECODRDBUSHUB_H
#define DECODRDBUSHUB_H

#include "org.salvum.DecodrHubAdp.h"
#include "decoderdbuscontroller.h"
#include "models/workspacemodel.h"
#include <ui/qobjectlistmodel.h>
#include <QPair>
#include <QMap>

class DecodrDbusHub : public Ui::QObjectListModel<DecoderDbusController>
{
    Q_OBJECT
public:
    DecodrDbusHub(QObject *parent =nullptr);
    ~DecodrDbusHub();

    void setWorkspaceModel(WorkspaceModel *wspace) { mWspace = wspace; }
    
    Q_INVOKABLE int getRewindCluster() const;
    
public slots:
    QDBusObjectPath aquireClient(int clientId);
    void releaseClient(int clientId);   
    
    void createDecoders(const QString &shmemPath, const QList<int> &heads);
    void createDecoders(const QString &shmemPath, const QVariant &heads);
    
    void startProcessing();
    
signals:
    void allDecodersConnected();
    
private slots:
    void decodrConnected(bool connected);

private:    
    const DecoderDbusController *decoderByClientId(int clientId) const;

    WorkspaceModel *mWspace;
    QList<int> mHeads;
    QString mShmemPath;
};

#endif
