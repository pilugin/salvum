#ifndef JPEG_PHELGMATICCHECKER_H
#define JPEG_PHELGMATICCHECKER_H

#include <jpeg/icheck.h>

namespace Jpeg {

class PhlegmaticChecker : public ICheck
{
public:
    PhlegmaticChecker(QObject *parent =nullptr) : ICheck(parent) {}
    bool check(const QImage &, int, int, double *relevance =nullptr)
    {
        if (relevance)
            *relevance = 1.0;
        return true;
    }
};

}

#endif // JPEG_PHELGMATICCHECKER_H
