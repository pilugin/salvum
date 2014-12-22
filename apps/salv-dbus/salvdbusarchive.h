#ifndef SALVDBUSARCHIVE_H
#define SALVDBUSARCHIVE_H

#include <QObject>
#include <jpeg/common.h>
#include <core-3/archive.h>
#include <common/types.h>

class SalvDbusArchive : public QObject, public Core3::Archive<Jpeg::DecodrState>
{
    Q_OBJECT
public:
    SalvDbusArchive(QObject *parent =nullptr);
    ~SalvDbusArchive();

signals:
    void initialized(const Common::ImageInfo &imageInfo);
    void clusterDecoded(const Common::DecodedClusterInfo &info, const Common::Pixels &pixels, bool isOk);

private:
    void onInitialized();
    void onStateSet(const Common::Cluster &cluster, const Jpeg::DecodrState &decodrState, bool isOk);
    void archiveBaselined(const Common::Cluster &cluster);

    struct Private;
    Private *m_d;
};

#endif // SALVDBUSARCHIVE_H
