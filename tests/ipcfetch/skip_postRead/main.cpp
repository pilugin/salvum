#include <ipcfetch/broadcast.h>
//#include <ipcfetch/recieverfetch.h>
#include <ipcfetch/feedbackresults.h>

#include <simplelogger.h>
#include <QtCore>
#include <QtDebug>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

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
        qDebug()<<"StartTest  :"<<reg;
        
        IPCFetch::BroadcastMessage bm;
        bm.status = IPCFetch::AtEnd;
        postRead(bm);
    }
};

int main(int , char **)
{
    const int forkCount = 4;
    const int procCount = 2 << (forkCount -1);

    SimpleLogger l;

    const char *shname = "/yobo";
    const char *shnamefb = "/yobo_fb";
    QVector<int> v;

    int rv = fork();
    if (rv == -1) {
        perror("fork");
        return -1;
    }

    if (rv > 0) {
        TestFetch *f = new TestFetch;
        TestBroadcast bc(shname, shnamefb, f);
        if (! bc.isValid()) {
            qDebug("Invalid BC");
            return -2;
        }

        qDebug("BC - OK");

        sleep(1);
        bc.startTest(procCount);
        bc.startTest(procCount);
        bc.startTest(procCount);
        bc.startTest(procCount);

        qDebug()<<"Test finished:";
        qDebug()<<"skipped.size()"<<f->skipped.size();

        for (int i=0; i<procCount; ++i) {
            int status;
            wait(&status);
//            qDebug() << "Child exited: "<<pid<<status;

        }
    
        qDebug("Sleep 5");
        sleep(5);
        qDebug("Exit");

    } else {
        for (int i=0; i<forkCount; ++i)
            fork();

        for (int i=0; i<10000; ++i)
            v.push_back(i);
        
        IPCFetch::FeedbackResults fr(shnamefb);
        if (!fr.isValid()) {
            qDebug("Invalid FR");
            return -3;
        }

        qDebug("FR - OK");       
        fr.restart(QString("%1").arg(getpid()));

        qDebug("FR - start skip");   
        fr.addClusters(v);
        qDebug("FR - done %d", getpid());
        fr.finilize(true);
    }


    return 0;
}
