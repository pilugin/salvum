#include <jpeg/archive.h>
#include <jpeg/thumbnailcreator.h>

namespace Jpeg {

Archive::Archive(const QString &path)
    : RangeFileResult(path)
{
    restart();
}

Archive::~Archive()
{
    finalize(true);
}

void Archive::onInitialized()
{
    QString thumbnailPath = QString("%1/thumbnail.jpg").arg(mDir);
    mDataFile.close();
    if (ThumbnailCreator::create(mDataFile.fileName(), thumbnailPath))
        emit thumbnailCreated(thumbnailPath);
    mDataFile.open(QFile::WriteOnly);
}

void Archive::archiveBaselined(const Common::Cluster &cluster)
{
    qDebug("Archive::archiveBaselined(%08X)", cluster.first);
    addCluster(cluster);
}

} // namespace Jpeg
