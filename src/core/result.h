#ifndef CORE_RESULT_H
#define CORE_RESULT_H

#include <QObject>

class Result : public QObject
{
    Q_OBJECT
public:
    Result(QObject *parent =nullptr);
    
public slots:
    virtual void restart(const QString &sessionName) =0;
    virtual void addClusters(int clusterNo, int length) =0;
    virtual void finalize(bool success) =0;
};

#endif // CORE_RESULT_H
