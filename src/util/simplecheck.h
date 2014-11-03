#ifndef UTIL_SIMPLE_CHECK_H
#define UTIL_SIMPLE_CHECK_H

#include <core3/check.h>

template <class DecodrState>
class SimpleCheck : public Core3::Check<DecodrState>
{
public:
    SimpleCheck(bool verbose =false, QObject *parent =nullptr);

    void doAcceptFrame(const QVector<int> &pendingClusters, const Core::DecodrFrame &frame);
    void doRejectFrame(const QVector<int> &pendingClusters);
    FrameDescription_itr chooseBaseline(const FrameDescription_v &frames);
private:
    const bool mVerbose;
};

#endif // UTIL_SIMPLE_CHECK_H
