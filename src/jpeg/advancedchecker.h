#ifndef ADVANCEDCHECKER_H
#define ADVANCEDCHECKER_H

#include "if/jpeg/icheck.h"

namespace Jpeg {

class AdvancedChecker : public ICheck
{
public:
    AdvancedChecker();
    bool check(const QImage &image, int blockBegin, int blockEnd, double *relevance =nullptr);
    double minRelevance() const;

private:
    double processRow(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;

    double processRow_Xtremum(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
    double processRow_Colors(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
};

} // eons Jpeg

#endif // ADVANCEDCHECKER_H
