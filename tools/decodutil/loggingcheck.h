#ifndef LOGGINGCHECK_H
#define LOGGINGCHECK_H

#include <core/check.h>
#include <dbus/dbustypes.h>

class LoggingCheck : public Check
{
    Q_OBJECT
public:
    explicit LoggingCheck(QObject *parent = 0);

    const DecodedClusters &res() const { return mRes; }
protected:
    const FrameDescription &chooseBaseline(const FrameDescription_v &frames);

    DecodedClusters mRes;
};

#endif // LOGGINGCHECK_H
