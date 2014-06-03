#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#include <if/ilog.h>
#include <stdio.h>

class SimpleLogger : public ILog
{
public:
    SimpleLogger();
    bool setSession(const QString &session);
    bool log(const QString &msg);
private:
    FILE *mFd;
};

#endif // SIMPLELOGGER_H