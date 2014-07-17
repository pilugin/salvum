#ifndef DECODECTRL_H
#define DECODECTRL_H

#include <QList>
#include <QPair>

class IDecod;
class IResults;
class IFetch;

class DecodeCtrl
{
public:
    DecodeCtrl();

    bool decode(IDecod *decodr, IFetch *fetch, IResults *results, int clusterNo);
    bool decode(IDecod *decodr, IFetch *fetch, IResults *results, const QList<QPair<int, int>> &rewinds);
protected:

    bool startDecode(IDecod *decodr, IFetch *fetch, IResults *results);
    bool proceedDecode(IDecod *decodr, IFetch *fetch, IResults *results);

    bool rewind(IFetch *fetch);
private:
    QList<QPair<int, int>> mRewinds;
};

#endif // DECODECTRL_H
