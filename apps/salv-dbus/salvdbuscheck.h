#ifndef SALV_DBUS_CHECK_H
#define SALV_DBUS_CHECK_H

#include <core/check.h>
#include <common/types.h>

class SalvDbusCheck : public Core::Check
{
    Q_OBJECT
public:
    explicit SalvDbusCheck(QObject *parent);    
    ~SalvDbusCheck();
    
    bool isWaiting() const;
    void setWorkspacePath(const QString &path);
signals: 
    void atEnd(bool complete, const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::ImageInfo &ii);
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);
    
public slots:
    void baseline(int clusterNo);
    void breakEventLoop(int retcod =-1);
    
protected:
    FrameDescription_itr chooseBaseline(const FrameDescription_v &frames);
    void doAcceptFrame(const Common::Clusters &pendingClusters, const Core::DecodrFrame &frame);
private:        
    class Private;
    Private *m_d;
};

#endif // SALV_DBUS_CHECK_H