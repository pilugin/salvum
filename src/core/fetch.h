#ifndef CORE_FETCH_H
#define CORE_FETCH_H

#include <QObject>

class Fetch : public QObject
{
    Q_OBJECT
    
public:
    enum { InvalidClusterNo =-1 };
    
    Fetch(QObject *parent =nullptr);

    virtual bool atEnd() const =0;    
    virtual QByteArray bitmap() const { return QByteArray(); }
    
public slots:    
    virtual void fetch(int &clusterNo, QByteArray &cluster) =0;
    virtual bool rewind(int clusterNo =0) =0;
    virtual void skip(int clusterNo, int length) =0;
    virtual void fastfwd() =0;
    
signals:
    void fetched(int clusterNo);
    void end();
};

#endif // CORE_FETCH_H
