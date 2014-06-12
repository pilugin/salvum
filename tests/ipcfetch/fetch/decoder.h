#ifndef DECODER_H
#define DECODER_H

#include <if/idecod.h>

class Decoder : public IDecod
{
public:
    Decoder();
    bool restart(IFetch *f);
    bool decodeCluster();
    void revert() {}
    bool done() const { return mLast == -1; }
    QVector<int> usedClusters() const { return mUsedClusters; }
private:
    int first(const QByteArray &ar) const;
    int last(const QByteArray &ar) const;

    int mLast;
    int mCount;
    IFetch *mFetch;
    QVector<int> mUsedClusters;
};

#endif
