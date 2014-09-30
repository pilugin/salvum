#include "util/filelogger.h"

#include <QDir>
#include <QtDebug>

FileLogger::FileLogger(const QString &outputDir, bool unbuffered) 
: mOutputDir(outputDir)
, mUnbuffered(unbuffered)
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

    if (mOut.isOpen())
        mOut.close();

    mOut.setFileName(QString("%1/%2.log").arg(mOutputDir, session).toAscii());
    if (!mOut.open(QFile::WriteOnly | QFile::Truncate
                   | (mUnbuffered ? QFile::Unbuffered : QFile::WriteOnly)
                   )) {
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

