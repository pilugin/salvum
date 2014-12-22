#ifndef SALV_DBUS_CHECK_H
#define SALV_DBUS_CHECK_H

#include <QObject>
#include <jpeg/common.h>
#include <core-3/check.h>
#include <common/types.h>

class SalvDbusCheck : public QObject, public Core3::Check<Jpeg::DecodrState>
{
    Q_OBJECT
public:
    explicit SalvDbusCheck(QObject *parent);    
    ~SalvDbusCheck();
    
    bool isWaiting() const;
    void setWorkspacePath(const QString &path);
signals: 
    void atEnd(const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::ImageInfo &ii);
    void progress(int clustersDecoded, int blocksDecoded, int blocksTotal);
    
public slots:
    void baseline(int clusterNo);
    void breakEventLoop(int retcod =-1);
    
protected:
    SelectResult select(const Core3::Archive<Jpeg::DecodrState> &archive);

private:        
    class Private;
    Private *m_d;
};

#endif // SALV_DBUS_CHECK_H
