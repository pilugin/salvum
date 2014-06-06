#include "createtestmedia.h"

#include <if/isettings.h>

#include <QtCore>
#include <QtDebug>

using namespace Settings;

class Generator
{
public:
    Generator(int number, int total, int clusterSize, int size) : mTotal(total), mSize(size), mClusterSize(clusterSize), mCurrent(number), mCurrentSize(0)   {}
    QByteArray gen()
    {
        QByteArray res(mClusterSize, '\0');
        int *first = reinterpret_cast<int *>(res.data());
        int *last = reinterpret_cast<int *>(res.data() + res.size()) -1;

        *first = mCurrent;
        mCurrent += mTotal;
        *last = mCurrent;
        ++mCurrentSize;
        return res;
    }
    bool atEnd() const { return mCurrentSize >= mSize; }

private:

    const int mTotal;
    const int mSize;
    const int mClusterSize;
    int mCurrent;
    int mCurrentSize;
};

bool createTestMedia(const QString &mediaFile, const QString &bitmapFile, int /*pattern*/)
{
    static const long long int clusterSize =  Settings::Get(ClusterSize).toLongLong();
    static const int mediaSize = 10000; // amount of clusters;
    static const int genCount = 4;
    static const int genSize = mediaSize / (genCount * 2);

    qDebug()<<"mediaSize:"<<mediaSize<<", genCount:"<<genCount<<", genSize:"<<genSize;

    QFile media(mediaFile);
    QFile bitmap(bitmapFile);

    if (!media.open(QFile::WriteOnly | QFile::Truncate) || !bitmap.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug()<<media.errorString()<<media.fileName();
        qDebug()<<bitmap.errorString()<<bitmap.fileName();
        return false;
    }



    Q_ASSERT( (genCount*genSize) <= mediaSize );

    QVector<Generator *> gens;
    for (int i=0; i<genCount; ++i)
        gens.append(new Generator(i, genCount, clusterSize, genSize));

    if (! media.resize(clusterSize * mediaSize )) {
        qDebug()<<media.errorString()<<media.fileName();
        return false;
    }

    QByteArray b(mediaSize, '0');

    bool firstTry = true;    
    for (bool cont=true; cont; ) {
        
        cont = false;
        for (int i=0; i<gens.size(); ++i) {
            if (! gens[i]->atEnd()) {
                cont = true;
                QByteArray ar = gens[i]->gen();
                int offset = qrand() % b.size();
                int cursor=0;
                for (; offset>0; ++cursor, --offset) {
                    while (b[ cursor % b.size() ] != '0')
                        ++cursor;
                }
                while (b[ cursor % b.size() ] != '0')
                    ++cursor;

                cursor %= b.size();
                b[cursor] = '1';                      
                if (! media.seek( clusterSize * cursor )) {  
                    qDebug() << media.errorString() << media.fileName();
                    return false;
                }
                media.write(ar);
                if (firstTry)
                    qDebug("HEAD: %08X", cursor);
            }
        }
        firstTry = false;

    }
    
    media.close();   
    
    bitmap.write(b);
    bitmap.close();

    return true;
}