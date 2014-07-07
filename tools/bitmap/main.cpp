#include <QtCore>
#include <QtDebug>
#include <cstdio>


const char *create_cmd = "-create";
void create(QString filename, qint64 size)
{
    QFile output(filename);
    if (!output.open(QFile::WriteOnly|QFile::Truncate)) {
        qDebug()<<"Failed to create"<<filename<<output.errorString();
        return;
    }

    QByteArray arr;
    const int N=10*1024;
    arr.fill('1', N);
    for (qint64 i=0; i<size; )
        i+=output.write(arr.data(), qMin((qint64)arr.size(), size-i));
}

const char *add_clusters_cmd = "-add-clusters";
void add_clusters(QString filename, QString zeros, char s)
{
    QFile map(filename);
    if (!map.open(QFile::ReadWrite)) {
        qDebug()<<"Failed to open"<<filename<<map.errorString();
        return;
    }

    QFile z(zeros);
    if (!z.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<zeros<<map.errorString();
        return;
    }

    while (!z.atEnd()) {
        QByteArray arr = z.readLine();
        if (arr.size() == 0)
            break;
        if (arr.endsWith('\n'))
            arr.remove(arr.size()-1, 1);

        QList<QByteArray> range = arr.split(' ');
        qint64 from = range.front().toLongLong(0, 16);
        qint64 to = range.back().toLongLong(0, 16);


        map.seek(from);
        QByteArray data(1, s);
        for (qint64 i=from; i<=to; ++i) {
            map.write(data);
            qDebug()<<i;
        }
    }
}

const char *analize_cmd = "-analize";
void analize(QString filename)
{
    QFile map(filename);
    if (!map.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<filename<<map.errorString();
        return;
    }
    QMap<char, qint64> statistics;
    char c;
    while (!map.atEnd()) {
        map.read(&c, 1);
        ++statistics[c];
    }

    qDebug() << statistics;
}

const char *add_good_jpegs_cmd = "-add-good-jpegs";
void add_good_jpegs(QString filename, QString dir)
{
    QFile map(filename);
    if (!map.open(QFile::ReadWrite)) {
        qDebug()<<"Failed to open"<<filename<<map.errorString();
        return;
    }

    const int N = 4*1024;
    QDir d(dir);

    foreach (QFileInfo fi, d.entryInfoList(QStringList()<<"*.jpg", QDir::NoDotAndDotDot | QDir::Files)) {
        qint64 offset = fi.fileName().split(".").front().toLongLong(0, 16);
        qint64 size = fi.size()/N + (fi.size()%N == 0 ? 0: 1);

        qDebug()<<fi.fileName()<<offset <<"    "<<size;

        if (map.size() < offset) {
            qDebug() << "SKIPPED";
            continue;
        }

        map.seek(offset);
        map.write("G", 1);
        for (qint64 i=1; i<size; ++i)
            map.write("g", 1);
    }
}

const char *set_clusters_cmd = "-set-clusters";
void set_clusters(QString filename, QString clusters, char value)
{
    QFile b(filename);
    if (!b.open(QFile::ReadWrite)) {
        qDebug()<<"Failed to open"<<filename<<b.errorString();
        return;
    }
    QFile c(clusters);
    if (!c.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<clusters<<c.errorString();
        return;
    }

    while (!c.atEnd()) {
        QByteArray arr = c.readLine();
        if (arr.endsWith('\n'))
            arr.remove(arr.size()-1, 1);
        qint64 i = arr.toLongLong(0, 16);
        if (i >= b.size())
            continue;
        b.seek(i);
        b.write(&value, 1);
    }
}

const char *extract_cmd = "-extract";
void extract(QString filename, QString output, char c)
{
    QFile b(filename);
    if (!b.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<filename<<b.errorString();
        return;
    }
    QFile o(output);
    if (!o.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug()<<"Failed to open"<<output<<o.errorString();
        return;
    }

    const int N = 4*1024;
    qint64 i=0;
    while (!b.atEnd()) {
        QByteArray arr = b.read(N);
        for (int j=0; j<arr.size(); ++j, ++i)
            if (arr[j] == c)
                o.write(QString("%1\n").arg(QString::number(i, 16)).toAscii());
    }
}

const char *calc_hash_cmd = "-calc-hash";
void calc_hash(QString bitmap, QString device, QString output, QByteArray clusters)
{
    QFile b(bitmap);
    if (!b.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<bitmap<<b.errorString();
        return;
    }
    QFile d(device);
    if (!d.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<device<<d.errorString();
        return;
    }
    QFile o(output);
    if (!o.open(QFile::WriteOnly| QFile::Truncate)) {
        qDebug()<<"Failed to open"<<output<<o.errorString();
        return;
    }

    const int N=4*1024;

    QByteArray out;
    out.reserve(sizeof(uint) + sizeof(qint64));
    QByteArray arr;
    arr.reserve(N);
    QByteArray cluster;
    cluster.reserve(N);

    qint64 offset=0;
    while (!b.atEnd()) {
        arr = b.read(N);
        for (int i=0; i<arr.size(); ++i)
            if (clusters.contains(arr[i])) {
                d.seek((i + offset) *N);
                cluster = d.read(N);
                if (cluster.size() == N) {
                    uint hash = qHash(cluster);
                    qint64 offset_ = i+offset;
                    out = QString().sprintf("%08X%016llX\n", hash, offset_).toAscii();
                    o.write(out);

                } else {
                    qDebug()<<"Cluster read error"<<cluster.size();
                }
            }

        offset+= arr.size();

        if ((offset %(100*N)) == 0) {
            qDebug()<<offset;
        }
    }
}

const char *dump_cmd = "-dump";
void dump(QString bitmap, QString device, QString output, QByteArray clusters)
{
    QFile b(bitmap);
    if (!b.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<bitmap<<b.errorString();
        return;
    }
    QFile d(device);
    if (!d.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<device<<d.errorString();
        return;
    }
    QFile o(output);
    if (!o.open(QFile::WriteOnly| QFile::Truncate)) {
        qDebug()<<"Failed to open"<<output<<o.errorString();
        return;
    }

    const int N=4*1024;

    QByteArray arr;
    arr.reserve(N);
    QByteArray cluster;
    cluster.reserve(N);

    qint64 offset = 0;
    while (!b.atEnd()) {
        arr = b.read(N);
        for (int i=0; i<arr.size(); ++i)
            if (clusters.contains(arr[i])) {
                d.seek((i + offset) *N);
                cluster = d.read(N);
                if (cluster.size() == N) {
                    o.write(cluster);

                } else {
                    qDebug()<<"Cluster read error"<<cluster.size();
                }
            }

        offset+= arr.size();

        if ((offset %(100*N)) == 0) {
            qDebug()<<offset;
        }
    }
}

const char *print_cmd = "-print";
void print(QString bitmap, char cluster)
{
    QFile b(bitmap);
    if (!b.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open"<<b.errorString();
        return;
    }

    QByteArray ar = b.readAll();
    for (int i=0; i<ar.size(); ++i)
        if (ar[i] == cluster)
            printf("%08X\n", i);

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc == 4 && QString(create_cmd) == argv[1]) {
        create(argv[2], QString(argv[3]).toLongLong());
        return 0;

    } else if (argc == 5 && QString(add_clusters_cmd) == argv[1]) {
        add_clusters(argv[2], argv[3], argv[4][0]);
        return 0;

    } else if (argc == 4 && QString(add_good_jpegs_cmd) == argv[1]) {
        add_good_jpegs(argv[2], argv[3]);
        return 0;

    } else if (argc == 3 && QString(analize_cmd) == argv[1]) {
        analize(argv[2]);
        return 0;

    } else if (argc == 5 && QString(set_clusters_cmd) == argv[1]) {
        set_clusters(argv[2], argv[3], argv[4][0]);
        return 0;

    } else if (argc == 5 && QString(extract_cmd) == argv[1]) {
        extract(argv[2], argv[3], argv[4][0]);
        return 0;

    } else if (argc == 6 && QString(calc_hash_cmd) == argv[1]) {
        calc_hash(argv[2], argv[3], argv[4], argv[5]);
        return 0;

    } else if (argc == 6 && QString(dump_cmd) == argv[1]) {
        dump(argv[2], argv[3], argv[4], argv[5]);

    } else if (argc == 4 && QString(print_cmd) == argv[1]) {
        print(argv[2], argv[3][0]);

    } else {
        qDebug()<<"Usage:";
        qDebug()<<"\t" << argv[0] << " " << create_cmd << " <bitmap-filename> <size in clusters>";
        qDebug()<<"\t" << argv[0] << " " << add_clusters_cmd << " <bitmap-filename> <zeros-filename> <Symbol>";
        qDebug()<<"\t" << argv[0] << " " << add_good_jpegs_cmd << " <bitname-filename> <good-jpegs-dir>";
        qDebug()<<"\t" << argv[0] << " " << analize_cmd << " <bitmap-filename>";
        qDebug()<<"\t" << argv[0] << " " << set_clusters_cmd << " <bitmap-filename> <clusters-filename> <Symbol>";
        qDebug()<<"\t" << argv[0] << " " << extract_cmd << " <bitmap-filename> <outfile> <Symbol>";
        qDebug()<<"\t" << argv[0] << " " << calc_hash_cmd << " <bitmap-filename> <device> <outfile> <cluster-types>";
        qDebug()<<"\t" << argv[0] << " " << dump_cmd << " <bitmap-filename> <device> <outfile> <cluster-types>";
        qDebug()<<"\t" << argv[0] << " " << print_cmd << " <bitmap-filename> <cluster-type>";
    }
    

    //return a.exec();



    return 0;
}
