#ifndef ADVANCEDCHECKER_H
#define ADVANCEDCHECKER_H

#include "if/jpeg/icheck.h"

class AdvancedChecker : public Jpeg::ICheck
{
public:
    AdvancedChecker();
    bool check(const QImage &image, int blockBegin, int blockEnd);

private:
    double processRow(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;

    double processRow_Xtremum(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
    double processRow_Colors(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
};

#endif // ADVANCEDCHECKER_H
