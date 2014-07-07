#include <QtCore>
#include <QtDebug>

const int N = 4*1024;

typedef QMultiMap<quint32, qint64> Index;

void fillIndex(QFile &h, Index &index)
{
    qint64 i=0;
    QByteArray buf;
    while (!h.atEnd()) {
        buf = h.readLine();
        quint32 key = buf.left(8).toUInt(0, 16);
        qint64 value = buf.mid(8, 16).toLongLong(0, 16);
//        qDebug("%08X %016llX %s", key, value, buf.data());
        index.insert(key, value);

        if ((++i % 10240) == 0) {
            qDebug()<<"read hash"<<h.pos()<<"/";
            qDebug()<<"         "<<h.size()<<".";
        }

    }
}

bool find(const QByteArray &jpgcluster, const Index &i, QFile &d, qint64 &from)
{
    quint32 h = qHash(jpgcluster);
    for(Index::const_iterator itr = i.find(h); itr!=i.end() && itr.key()==h; ++itr) {
        if (!d.seek(*itr * N)) {
            qDebug()<<"Seek failed"<<d.errorString();
            return false;
        }

        QByteArray buf = d.read(jpgcluster.size());
        if (buf == jpgcluster) {
            from = *itr;
            return true;
        }
    }

    qDebug("%%(   %08X", h);
    return false;
}

bool comparefurther(QFile &d, QFile &jpg, QByteArray &jpgcluster, qint64 &to)
{
    QByteArray buf;
    do {
        ++to;
        jpgcluster = jpg.read(N);
        buf = d.read(jpgcluster.size());
    } while (jpgcluster == buf && jpgcluster.size()>0);
    --to;

    return jpg.atEnd();
}

void allocate(const Index &i, QFile &d, QFile &jpg, QFile &out)
{
    QByteArray outbuf;

    outbuf = jpg.read(N);

    qint64 from = 0;
    qint64 to   = 0;
    while (true) {

        if (find(outbuf, i, d, from)) {
            to = from;
            bool r = comparefurther(d, jpg, outbuf, to);

            out.write(QString().sprintf("%016llX %016llX\n", from, to).toAscii());

            if (r)
                return;

        } else {
            qDebug()<<"Failed to find by index"<<jpg.fileName()<<((jpg.pos() - 1) / N);
            out.close();
            out.remove();
            //outbuf = jpg.read(N);
            return;
        }
    }
}

void act(QString device, QString hash)
{
    QFile d(device);
    QFile h(hash);
    QFile in;

    Index index;

    if (!d.open(QFile::ReadOnly) || !h.open(QFile::ReadOnly) || !in.open(STDIN_FILENO, QFile::ReadOnly)) {
        qDebug()<<d.errorString();
        qDebug()<<h.errorString();
        qDebug()<<in.errorString();
        return;
    }

    fillIndex(h, index);
    h.close();

    qDebug()<<"Index filled";

    QByteArray input;
    QByteArray outfilename;
    while (true) {
        qDebug()<<"INPUT::";
        input = in.readLine();

        if (input == "EOF\n")
            break;

        input.remove(input.size()-1, 1); // remove \n
        QFile jpg(input);
        outfilename = input + ".clusters";
        outfilename.replace("/", "_");
        QFile out(outfilename);
        if (!jpg.open(QFile::ReadOnly) || !out.open(QFile::WriteOnly|QFile::Truncate|QFile::Unbuffered)) {
            qDebug()<<"Failed to open"<<input<<jpg.errorString();
            qDebug()<<out.errorString();
            continue;
        }

        allocate(index, d, jpg, out);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc == 3) {
        act(argv[1], argv[2]);

    } else {
        qDebug()<<"Usage: "<<argv[0]<<" <device> <hashs>";
    }
    
    return 0;//a.exec();
}
