#ifndef LOGGINGCHECK_H
#define LOGGINGCHECK_H

#include <core/check.h>
#include <common/types.h>

class LoggingCheck : public Core::Check
{
    Q_OBJECT
public:
    explicit LoggingCheck(QObject *parent = 0);

    const Common::DecodedClusters &res() const { return mRes; }
protected:
    const FrameDescription &chooseBaseline(const FrameDescription_v &frames);

    Common::DecodedClusters mRes;
};

#endif // LOGGINGCHECK_H
