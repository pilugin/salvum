#ifndef BENCH_H
#define BENCH_H

#include <QVector>
#include <QByteArray>
#include <QFile>
#include <QPair>

#include "if/jpeg/icheck.h"
#include "if/ifetch.h"

namespace Jpeg {

class Bench : public IFetch, public ICheck
{
public:
    Bench();
    
    bool init(ICheck *testee, const QString &clustersPath);
    
    // ifetch impl
    bool rewind(int,int) { return true; }
    void skip(int, int) {}
    void fastfwd() {}
    void fetch(int &clusterNo, QByteArray &cluster);
    bool atEnd() const;
    

    // icheck impl
    bool check(const QImage &image, int blockBegin, int blockEnd, double *relevance =nullptr);
    double minRelevance() const;

    const QVector<QPair<int,double>> checkRelevances() const { return mCheckRelevances; } 

private:
    ICheck *mTestee;
    QFile mInput;
    QVector<QPair<int, double>> mCheckRelevances;
    int mOffset;
};


} // eons Jpeg

#endif