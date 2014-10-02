#ifndef CORE_FETCH_H
#define CORE_FETCH_H

#include <QObject>

namespace Core {

class Fetch : public QObject
{
    Q_OBJECT
    
public:
    enum { InvalidClusterNo =-1 };
    
    Fetch(QObject *parent =nullptr);

    virtual bool atEnd() const =0;    
    virtual QByteArray bitmap() const { return QByteArray(); }
    
public slots:    
    void fetch(int &clusterNo, QByteArray &cluster);
    virtual bool rewind(int clusterNo =0) =0;
    virtual void skip(int clusterNo, int length) =0;
    virtual void fastfwd() =0;
    
signals:
    void fetched(int clusterNo, const QByteArray &cluster);
    void end();
    
protected:
    virtual void doFetch(int &clusterNo, QByteArray &cluster) =0;
};

}

#endif // CORE_FETCH_H
