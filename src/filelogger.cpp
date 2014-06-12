#include "filelogger.h"

#include <QDir>
#include <QtDebug>

FileLogger::FileLogger(const QString &outputDir) : mOutputDir(outputDir)
{
}

FileLogger::~FileLogger()
{
}

bool FileLogger::setSession(const QString &session)
{
    if (!QDir().mkpath(mOutputDir)) {
        qDebug()<<"Failed to create dir"<<mOutputDir;
        return false;
    }

    mOut.setFileName(QString("%1/%2.log").arg(mOutputDir, session).toAscii());
    if (!mOut.open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered)) {
        qDebug()<<mOut.errorString();
        return false;
    }

    return true;
}

bool FileLogger::log(const QString &msg)
{
    QByteArray ascii = msg.toAscii();
    return mOut.write( ascii ) == ascii.size();
}

