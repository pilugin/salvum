#include "bench.h"
#include "if/jpeg/icheck.h"
#include "if/ifetch.h"
#include "if/iresults.h"
#include "jpeg/picojpegdecodr.h"
#include "jpeg/advancedchecker.h"

class Fetch : public IFetch
{
public:
    Fetch(const QVector<ClusterInput> &input) : mInput(input), mCurrent(0) {}
    bool rewind(int,int) { return true; }
    void skip(const QVector<int> &) {}
    void fastfwd() {}
    
    bool atEnd() const { return mCurrent>=0 && mCurrent<mInput.size(); }
    void fetch(int &clusterNo, QByteArray &cluster) 
    { 
        if (atEnd()) {
            clusterNo = InvalidClusterNo;
            cluster.clear();
        } else {
            clusterNo =     mInput[mCurrent].clusterNo;
            cluster =       mInput[mCurrent].cluster;
            ++ mCurrent;
        }
    }
  
private:
    const QVector<ClusterInput> &mInput;
    int mCurrent;
};






QVector<ClusterCheckInfo> runCheckDecod(const QVector<ClusterInput> &input, Jpeg::ICheck &checkr)
{
    return QVector<ClusterCheckInfo>();
}
