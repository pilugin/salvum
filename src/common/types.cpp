#include <common/types.h>
#include <util/isettings.h>

static struct QtMetatypesRegistrer
{
    QtMetatypesRegistrer()
    {
        qRegisterMetaType<Common::Cluster>("Common::Cluster");
        qRegisterMetaType<Common::Clusters>("Common::Clusters");
        qRegisterMetaType<Common::ImageInfo>("Common::ImageInfo");
        qRegisterMetaType<Common::DecodedClusterInfo>("Common::DecodedClusterInfo");
        qRegisterMetaType<Common::Pixels>("Common::Pixels");
        qRegisterMetaType<Common::Result>("Common::Result");
        qRegisterMetaType<Common::BitmapInfo>("Common::BitmapInfo");
    }
} theQtMetatypesRegistrer;

namespace Common {

BitmapInfo::BitmapInfo()
{
    jpegHeads = goodHeads = zeros = goods = decodable = starts = 0;
}    

}
