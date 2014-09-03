#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <util/ilog.h>
#include <QFile>

class FileLogger : public ILog
{
public:
    FileLogger(const QString &outputDir);
    ~FileLogger();

    bool setSession(const QString &session);
    bool log(const QString &msg);
private:
    const QString mOutputDir;

    QFile mOut;
};

#endif // FILELOGGER_H
