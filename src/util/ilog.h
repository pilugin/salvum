#ifndef ILOG_H
#define ILOG_H

#include <util/singleton.h>
#include <QString>

class ILog : public Singleton<ILog>
{
public:
    virtual bool setSession(const QString &session) =0;
    virtual bool log(const QString &msg) =0;
};




namespace Log {

inline void Session(const QString &session)
{
    ILog::instance()->setSession(session);
}

inline void Msg(const QString &str)
{
    ILog::instance()->log(str);
}

inline void Msg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    Msg(QString().vsprintf(format, ap));
    va_end(ap);
}

}

#endif
