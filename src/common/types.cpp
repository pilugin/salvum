#include <common/types.h>
#include <util/isettings.h>

static struct QtMetatypesRegistrer
{
    QtMetatypesRegistrer()
    {
        qRegisterMetaType<Common::Cluster>("Common::Cluster");
        qRegisterMetaType<Common::Clusters>("Common::Clusters");
        qRegisterMetaType<Common::Pixmap>("Common::Pixmap");
        qRegisterMetaType<Common::ImageInfo>("Common::ImageInfo");
        qRegisterMetaType<Common::DecodedClusterInfo>("Common::DecodedClusterInfo");
        qRegisterMetaType<Common::DecodedClusters>("Common::DecodedClusters");
        qRegisterMetaType<Common::RejectedClusterInfo>("Common::RejectedClusterInfo");
        qRegisterMetaType<Common::RejectedClusters>("Common::RejectedClusters");
        qRegisterMetaType<Common::Result>("Common::Result");
        qRegisterMetaType<Common::BitmapInfo>("Common::BitmapInfo");
    }
} theQtMetatypesRegistrer;

namespace Common {

BitmapInfo::BitmapInfo()
{
    jpegHeads = goodHeads = zeros = goods = decodable = starts = 0;
}    

QList<int>::const_iterator RejectedClusterInfo::block(int blockNumber) const
{
    if ((blockNumber *8*8) >= pixels.size())
        return pixels.end();
            
    return pixels.constBegin() + (blockNumber *8*8);
}

int RejectedClusterInfo::pixel(int blockNumber, int x, int y) const
{
    auto itr = block(blockNumber);
    if (x<0 || y<0 || x>7 || y>7 || itr==pixels.end())
        return 0; //< assert?
        
    return *(itr +x +y*8);
}

}
