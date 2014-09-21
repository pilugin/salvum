#ifndef SALV_DBUS_CHECK_H
#define SALV_DBUS_CHECK_H

#include <core/check.h>
#include <common/types.h>
#include <QEventLoop>

class SalvDbusCheck : public Core::Check
{
    Q_OBJECT
public:
    SalvDbusCheck(QObject *parent =nullptr);
    
signals: 
    void atEnd(bool complete, const Common::DecodedClusters &decodedClusters, const Common::RejectedClusters &rejectedClusters, const Common::Pixmap &pixmap);
    
public slots:
    void baseline(int clusterNo);
    
protected:
    FrameDescription_itr chooseBaseline(const FrameDescription_v &frames);
private:        
    QEventLoop *mEloop;
};

#endif // SALV_DBUS_CHECK_H