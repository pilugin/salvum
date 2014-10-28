#ifndef CORE_RESULT_H
#define CORE_RESULT_H

#include <QObject>
#include <QPair>
#include <QByteArray>
#include <QVector>

#include <common/types.h>

namespace Core {

class Result : public QObject
{
    Q_OBJECT
public:
    Result(QObject *parent =nullptr);
    
public slots:
    virtual void restart(const QString &sessionName) =0;
    virtual void addClusters(const Common::Clusters &clusters) =0;
    virtual void finalize(bool success) =0;
};

}

#endif // CORE_RESULT_H
