#ifndef UTIL_SIMPLE_CHECK_H
#define UTIL_SIMPLE_CHECK_H

#include <core/check.h>

class SimpleCheck : public Check
{
public:
    SimpleCheck(bool verbose =false, QObject *parent =nullptr);

    void doAcceptFrame(const QVector<int> &pendingClusters, const DecodrFrame &frame);
    void doRejectFrame(const QVector<int> &pendingClusters);
    const FrameDescription &chooseBaseline(const FrameDescription_v &frames);
private:
    const bool mVerbose;
};

#endif // UTIL_SIMPLE_CHECK_H
