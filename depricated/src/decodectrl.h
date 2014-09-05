#ifndef DECODECTRL_H
#define DECODECTRL_H

class IDecod;
class IResults;
class IFetch;

class DecodeCtrl
{
public:
    DecodeCtrl();

    bool decode(IDecod *decodr, IFetch *fetch, IResults *results, int clusterNo);
protected:

    bool startDecode(IDecod *decodr, IFetch *fetch, IResults *results);
    bool proceedDecode(IDecod *decodr, IFetch *fetch, IResults *results);

    bool rewind(IFetch *fetch, int clusterNo =0);
};

#endif // DECODECTRL_H
