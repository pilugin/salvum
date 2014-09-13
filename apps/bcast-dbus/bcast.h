#ifndef BCAST_H
#define BCAST_H

#include <ipcfetch/broadcast.h>
#include <common/types.h>
#include <util/slotclosure.h>

class Bcast : public IPCFetch::Broadcast
{
public:
    typedef IPCFetch::Broadcast Super;

    Bcast(const char *shmem, const SlotClosure &progressCallback, const SlotClosure &bitmapInfo, Core::Fetch *fetch =nullptr);

    void emitProgress();
    void emitBitmapInfo();
    void skip(int clusterNo, int length);
    
    static void bitmapInfo(const QByteArray &bitmap, QList<int> &jpegHeads, QList<int> &goodHeads, Common::BitmapInfo &info);
    
private:
    enum InternalOperation 
    {
        EmitProgress = Super::Custom,
        EmitBitmapInfo,
        Skip,
        Custom
    };

    bool processInternalMsg(int internalMsg);
    void doEmitProgress();
    void doEmitBitmapInfo();
    void postRead(const IPCFetch::BroadcastMessage &message);
    

    

    struct { int clusterNo, length; } mSkip;
    struct { int currentCluster, clusterCount; } mProgress;
    SlotClosure mProgressCallback;
    SlotClosure mBitmapInfoCallback;
};

#endif // BCAST_H
