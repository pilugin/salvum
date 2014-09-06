#include "loggingcheck.h"

LoggingCheck::LoggingCheck(QObject *parent) :
    Check(parent)
{
}

void LoggingCheck::doAcceptFrame(const QVector<int> &pendingClusters, const DecodrFrame &frame)
{

}

const Check::FrameDescription &LoggingCheck::chooseBaseline(const Check::FrameDescription_v &frames)
{
    return frames.back();
}
