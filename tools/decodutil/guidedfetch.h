#ifndef GUIDEDFETCH_H
#define GUIDEDFETCH_H

#include <core/fetch.h>
#include <QString>
#include <QFile>
#include <QList>

class GuidedFetch : public Fetch
{
    Q_OBJECT
public:
    explicit GuidedFetch(const QString &filename, const QList<int> &clusters, QObject *parent = 0);

    virtual bool atEnd() const;
    void fetch(int &clusterNo, QByteArray &cluster);
    bool rewind(int  =0);
    void skip(int , int ) {}
    void fastfwd() {}

private:
    QFile mFile;
    QList<int> mClusters;
};

#endif // GUIDEDFETCH_H
