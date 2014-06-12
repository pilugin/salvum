#include <util/ipc.h>
#include <rdwr/reader.h>
#include <rdwr/writer.h>
#include <QtCore>
#include <QtDebug>

#include <unistd.h>
#include <stdlib.h>

struct S { int a; };

class Rd : public RdWr::Reader<S>
{
public:
    Rd() : RdWr::Reader<S>("/yobo") {}
protected:
    bool process(const S &data) 
    {

        qDebug()<<" READ"<<data.a;
        sleep(1);
        return data.a < 10;
    }    

};

class Wr : public RdWr::Writer<S>
{
public:
    Wr() : RdWr::Writer<S>("/yobo"), a(0) {}
protected:
    bool prepare(S &data)
    {
        qDebug()<<"WRITE"<<a;
        data.a = a++;

        sleep(1);
        return data.a < 10;
    }
    int a;
};



int main(int , char **)
{
    IPC::Container<QString, 3> strs;

    strs.get<0>() = "0";
    strs.get<1>() = "1";
    strs.get<2>() = "2";


    IPC::SynchroMem< S, 1, 2 > * sm = IPC::SynchroMem< S, 1, 2>::create("/yobo");
    (void)sm;

    qDebug() << strs.get<0>() << strs.get<1>() << strs.get<2>() << std::is_pod<IPC::SynchroMem< S, 1, 2> > ::value
            << std::is_pod< IPC::Mutex >::value;

//    sm->mutex<0>().lock();
//    sm->cond<1>().wait(  sm->mutex<0>() );

    int f = fork();
    if (f == -1) {
        perror("fork");
        return -1;
    }

    if (f == 0) {   
    
        qDebug("Start writer");
        Wr writer;

       

        writer.write();
        //for (;;);

    } else {    
        

        sleep (1);
        fork();
        fork();

        qDebug("Start reader");
        Rd reader;
	reader.reg();
        reader.read();
	reader.unreg();
    }

    
    //reader.read();

    return 0;
}
