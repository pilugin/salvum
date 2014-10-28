#ifndef JPEG_THUMBNAIL_CREATOR_HPP
#define JPEG_THUMBNAIL_CREATOR_HPP

#include <QObject>
#include <QTemporaryFile>

namespace Core {
    class Fetch;
    class Decodr;
}

namespace Jpeg {

class AdvancedChecker;

class ThumbnailCreator : public QObject
{
    Q_OBJECT
public:
    ThumbnailCreator(QObject *parent =nullptr);
    
    void init(Core::Fetch *fetch, Core::Decodr *decodr, AdvancedChecker *checker);
    void setSelfDelete();
    
    void start(const QString &outputPath);
    
signals:
    void thumbnailCreated(const QString &path);
private slots:
    void onDecodrAccepted();
    void onFetched(int, const QByteArray &cluster);
    
private:
    QTemporaryFile mJhead;
    QString mOutputPath;
    bool mSuicide;
};

}

#endif // JPEG_THUMBNAIL_CREATOR_HPP