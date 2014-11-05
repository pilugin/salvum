#include <jpeg/archive.h>
#include <jpeg/thumbnailcreator.h>

namespace Jpeg {

Archive::Archive(const QString &path)
    : RangeFileResult(path)
{
    restart("clusters", "data.jpg");
}

Archive::~Archive()
{
    finalize(true);
}

void Archive::onInitialized()
{
    QString thumbnailPath = QString("%1/thumbnail.jpg").arg(mDir);
    mDataFile.flush();
    if (ThumbnailCreator::create(mDataFile.fileName(), thumbnailPath))
        emit thumbnailCreated(thumbnailPath);

}

void Archive::archiveBaselined(const Common::Cluster &cluster)
{
//    qDebug("Archive::archiveBaselined(%08X)", cluster.first);
    addCluster(cluster);
}

} // namespace Jpeg
