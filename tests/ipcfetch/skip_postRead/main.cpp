#include <ipcfetch/broadcast.h>
#include <ipcfetch/recieverfetch.h>
#include <QtCore>
#include <QtDebug>
#include <unistd.h>
#include <stdlib.h>

class TestFetch : public IFetch
{
public:
    bool rewind(int,int) { return true; }
    void fastfwd() {}
    void fetch(int &, QByteArray &) {}
    bool atEnd() const { return true; }
    void skip(const QVector<int> &clusters) { skipped += clusters; }
    QVector<int> skipped;
};

class TestBroadcast : public IPCFetch::Broadcast
{
public:
    TestBroadcast(const char *n, const char *n2, IFetch *f) : IPCFetch::Broadcast(n, n2, f)     {}

    void startTest(int reg)
    {
        using namespace IPC;
        using namespace IPCFetch;
        using namespace RdWr;

        const Broadcast::ShMem &cshm = sharedMem();
        Broadcast::ShMem &shm = const_cast<Broadcast::ShMem &>(cshm);

        Mutexes<2> &m = shm.mutexes();
        Conds<2> &c =   shm.conds();

        m.mutex<REG>().lock();
        while (sharedMem().regCount < reg)
            c.cond<WR>().wait(  m.mutex<REG>() );

        qDebug("BC all registered");

        IPCFetch::BroadcastMessage bm;
        bm.status = IPCFetch::AtEnd;
    
        postRead(bm);

        m.mutex<REG>().unlock();
    }
};

int main(int argc, char **argv)
{
    const char *shname = "/yobo";
    const char *shnamefb = "/yobo_fb";
    QVector<int> v;
    for (int i=0; i<10000; ++i)
        v.push_back(i);

    int rv = fork();
    if (rv == -1) {
        perror("fork");
        return -1;
    }

    if (rv > 0) {
        TestFetch f;
        TestBroadcast bc(shname, shnamefb, &f);
        if (! bc.isValid()) {
            qDebug("Invalid BC");
            return -2;
        }

        qDebug("BC - OK");
        bc.startTest(16);

        qDebug()<<"Test finished:";
        qDebug()<<"skipped.size()"<<f.skipped.size();

    } else {
        sleep(1);
        fork();
        fork();
        fork();
        fork();
        
        IPCFetch::RecieverFetch rf(shname, shnamefb);
        if (! rf.isValid() ) {
            qDebug("Invalid RF");
            return -3;
        } 
        qDebug("RF - OK");
        
        rf.reg();

        sleep(1);

        qDebug("RF - start skip");
        rf.skip(v);

        rf.unreg();

    }


    return 0;
}