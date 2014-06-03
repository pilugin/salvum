#include "simplelogger.h"

SimpleLogger::SimpleLogger() : mFd(stderr)
{
}

bool SimpleLogger::setSession(const QString &session)
{
    fprintf(mFd, "\n{%s}\n", session.toUtf8().data() );
    return true;
}

bool SimpleLogger::log(const QString &msg)
{
    fprintf(mFd, "%s", msg.toUtf8().data() );
    return true;
}