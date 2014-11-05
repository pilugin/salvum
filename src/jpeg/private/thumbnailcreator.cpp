#include <jpeg/thumbnailcreator.h>

#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QStringList>

namespace Jpeg {

bool ThumbnailCreator::create(const QString &inputPath, const QString &outputPath, bool async)
{
    const char *jhead = "jhead";
    QStringList params;
    params << "-st" << outputPath << inputPath;
    if (!async) {
        int rv = QProcess::execute("jhead", QStringList() << "-st" << outputPath << inputPath );
        if (rv != 0) {
            qDebug()<<"jhead: returned nonzero..."<<rv;
            return false;
        }
        return QFile::exists(outputPath);


    } else {
        return QProcess::startDetached(jhead, params);

    }
}

}
