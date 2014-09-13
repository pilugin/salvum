#include <jpeg/algo.h>
#include <util/isettings.h>

using namespace Settings;
using namespace Common;

namespace Jpeg {

BitmapInfo processBitmap(const QByteArray &bm, QList<int> &jpegHeads, QList<int> &goodHeads)
{    
    BitmapInfo r;
    
    QByteArray goodClusters = Get(GoodClusters).toByteArray();
    char zero = Get(ZeroCluster).toChar().toAscii();
    char head = Get(JpegHead).toChar().toAscii();
    char goodHead = Get(GoodJpegHead).toChar().toAscii();
    char good = Get(GoodJpegCluster).toChar().toAscii();
    bool prevGood = false;
        
    for (int i=0; i<bm.size(); ++i) {
        char c = bm[i];
        if (goodClusters.contains(c)) {
            ++r.decodable;
            if (c == head) {
                ++r.jpegHeads;
                jpegHeads.push_back(i);
            }
            if (!prevGood)
                ++r.starts;                
            prevGood = true;
          
        } else {
            if (c == zero)
                ++r.zeros;
           else if (c == goodHead) {
                ++r.goodHeads;
                goodHeads.push_back(i);
           } else if (c == good)
                ++r.goods;                    
            prevGood = false;
        }
                        
    } // for
    
    return r;
}

}