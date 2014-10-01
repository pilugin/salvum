#ifndef DECODRDBUSHUB_H
#define DECODRDBUSHUB_H

#include "org.salvum.DecodrHubAdp.h"
#include "decoderdbuscontroller.h"
#include "models/workspacemodel.h"
#include <ui/qobjectlistmodel.h>
#include <QPair>
#include <QMap>
#include <functional>

namespace Ui {
class ImageProvider;
}

class DecodrDbusHub : public Ui::QObjectListModel<DecoderDbusController>
{
    Q_OBJECT
public:
    DecodrDbusHub(QObject *parent =nullptr);
    ~DecodrDbusHub();

    void setWorkspaceModel(WorkspaceModel *wspace) { mWspace = wspace; }
    void setImageProvider(Ui::ImageProvider *imgProv) { mImgProv = imgProv; }
    
    Q_INVOKABLE int getRewindCluster() const;
    Q_INVOKABLE int getUncheckedItemIndex(int currentIndex) const;
    
public slots:
    QDBusObjectPath aquireClient(int clientId);
    void releaseClient(int clientId);   
    
    void createDecoders(const QString &shmemPath, const QList<int> &heads);
    void createDecoders(const QString &shmemPath, const QVariant &heads);
    
    void startProcessing();
    void baselineDecoders();
    
signals:
    void allDecodersConnected();
    void allDecodersWaitForCheck();
    void allDecodersChecked();
    
    // for Decoders
    void sendBaseline();
    
private slots:
    void decodrConnected(bool connected);
    void decodingChanged(bool decoding);
    void checkedChanged(bool checked);

private:    
    bool allDone(std::function<bool (DecoderDbusController *)> f) const;
    const DecoderDbusController *decoderByClientId(int clientId) const;

    WorkspaceModel *mWspace;
    Ui::ImageProvider *mImgProv;
    QList<int> mHeads;
    QString mShmemPath;
};

#endif
