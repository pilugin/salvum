#ifndef CORE_RESULT_H
#define CORE_RESULT_H

#include <QObject>
#include <QPair>
#include <QByteArray>
#include <QVector>

namespace Core {

class Result : public QObject
{
    Q_OBJECT
public:
    Result(QObject *parent =nullptr);
    
public slots:
    virtual void restart(const QString &sessionName) =0;
    virtual void addClusters(const QVector<QPair<int,QByteArray>> &clusters) =0;
    virtual void finalize(bool success) =0;
};

}

#endif // CORE_RESULT_H
