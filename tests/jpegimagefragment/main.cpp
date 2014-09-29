#include <jpeg/imagehelpers.h>

int main(int argc, char **argv)
{
    Common::RejectedClusterInfo rc;
    
    const int W = 7;
    const int H = 34;
    const int B = 11;
    const int L = 14; //< len blocks
    
    rc.blockBegin = B;
    for (int i=0; i<L; ++i)
        for (int j=0; j<64; ++j)
            rc.pixels.push_back(qRgb(80,0,0));
    QImage img = Jpeg::imageFragment(W*8, H*8, rc);
    
    img.save("jpegimagefragment.png", "PNG");

    return 0;
}