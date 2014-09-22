#ifndef SALV_DBUS_CHECK_H
#define SALV_DBUS_CHECK_H

#include <core/check.h>
#include <common/types.h>

class SalvDbusCheck : public Core::Check
{
    Q_OBJECT
public:
    SalvDbusCheck(QObject *parent =nullptr);    
    ~SalvDbusCheck();
    
    bool isWaiting() const;
signals: 
    void atEnd(bool complete, const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);
    
public slots:
    void baseline(int clusterNo);
    void breakEventLoop(int retcod =-1);
    
protected:
    FrameDescription_itr chooseBaseline(const FrameDescription_v &frames);
private:        
    class Private;
    Private *m_d;
};

#endif // SALV_DBUS_CHECK_H