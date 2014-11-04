#ifndef JPEG_THUMBNAIL_CREATOR_HPP
#define JPEG_THUMBNAIL_CREATOR_HPP

class QString;

namespace Jpeg {

class ThumbnailCreator
{
public:
    /// if async then return value is always true
    static bool create(const QString &inputPath, const QString &outputPath, bool async=false);
};

}

#endif // JPEG_THUMBNAIL_CREATOR_HPP
