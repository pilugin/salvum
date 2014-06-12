#ifndef TESTDECODECTRL_H
#define TESTDECODECTRL_H

#include <decodectrl.h>

class TestDecodeCtrl : public DecodeCtrl
{
public:
    TestDecodeCtrl();

    using DecodeCtrl::decode;
private:
};

#endif // TESTDECODECTRL_H
