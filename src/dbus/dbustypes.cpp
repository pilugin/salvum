#include <dbus/dbustypes.h>
#include <util/isettings.h>

BitmapInfo::BitmapInfo()
{
    jpegHeads = goodHeads = zeros = goods = decodable = starts = 0;
}    
    
BitmapInfo BitmapInfo::processBitmap(const QByteArray &bm, QList<int> &jpegHeads, QList<int> &goodHeads)
{    
    BitmapInfo r;
    
    QByteArray goodClusters = Settings::Get(Settings::GoodClusters).toByteArray();
    char zero = Settings::Get(Settings::ZeroCluster).toChar().toAscii();
    char head = Settings::Get(Settings::JpegHead).toChar().toAscii();
    char goodHead = Settings::Get(Settings::GoodJpegHead).toChar().toAscii();
    char good = Settings::Get(Settings::GoodJpegCluster).toChar().toAscii();
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