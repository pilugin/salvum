#ifndef JPEG_ARCHIVE_H
#define JPEG_ARCHIVE_H

#include <core-3/archive.h>
#include <jpeg/common.h>
#include <util/rangefileresult.h>
#include <QObject>

namespace Jpeg {

class Archive : public QObject,
                public Core3::Archive< DecodrState >,
                private RangeFileResult
{
    Q_OBJECT
public:
    Archive(const QString &dir);
    ~Archive();

signals:
    void thumbnailCreated(const QString &path);

protected:
    void onInitialized();
    void archiveBaselined(const Common::Cluster &cluster);
};

} // namespace Jpeg

#endif // JPEG_ARCHIVE_H
