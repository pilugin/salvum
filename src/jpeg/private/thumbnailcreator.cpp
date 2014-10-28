#include <jpeg/thumbnailcreator.h>
#include <jpeg/advancedchecker.h>
#include <core/decodr.h>
#include <core/fetch.h>

#include <QtDebug>
#include <QProcess>
#include <QFile>

namespace Jpeg {

ThumbnailCreator::ThumbnailCreator(QObject *parent) : QObject(parent), mSuicide(false)
{
}
    
void ThumbnailCreator::init(Core::Fetch *fetch, Core::Decodr *decodr, AdvancedChecker *checker)
{
    connect(fetch, SIGNAL(fetched(int,QByteArray)), this, SLOT(onFetched(int,QByteArray)) );
    connect(decodr, SIGNAL(accepted(DecodrFrame)), this, SLOT(onDecodrAccepted())   );
    connect(this, SIGNAL(thumbnailCreated(QString)), checker, SLOT(addThumbnail(QString))   );
}

void ThumbnailCreator::start(const QString &outputPath)
{    
    mOutputPath = outputPath;
    if (!mJhead.open())
        qDebug()<<"Failed to open TMP file";
}

void ThumbnailCreator::setSelfDelete()
{
//    connect(this, SIGNAL(thumbnailCreated(QString)), this, SLOT(deleteLater())  );
    mSuicide = true;
}
    
void ThumbnailCreator::onDecodrAccepted()
{
    mJhead.close();
    
    int rv = QProcess::execute("jhead", QStringList() << "-st" << mOutputPath << mJhead.fileName() );
    if (rv != 0) 
        qDebug()<<"jhead: returned nonzero..."<<rv;
        
    if (QFile::exists(mOutputPath)) 
        emit thumbnailCreated(mOutputPath);
    if (mSuicide)
        delete this;        
}

void ThumbnailCreator::onFetched(int, const QByteArray &cluster)
{
    mJhead.write(cluster);
}


}
