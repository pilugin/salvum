#include "salvobjectcontroller.h"
#include "guidedfetch.h"
#include "loggingcheck.h"

#include <core/controller.h>
#include <util/simplecheck.h>
#include <jpeg/phlegmaticchecker.h>
#include <jpeg/picojpegdecodr.h>
#include <jpeg/advancedchecker.h>

#include <QtCore>
#include <QtDebug>

SalvObjectController::SalvObjectController(QObject *parent) :
    SalvJpegObject(1, QString("image://%1").arg(imageProviderName()), parent),
    mController(nullptr)
{
}

void SalvObjectController::processDecode(QString filename, QString clusterList)
{
    QString errorMsg;
    QList<int> clusters = translateClusterList(clusterList, errorMsg);
    qDebug()<<clusters;
    if (clusters.size()==0 && errorMsg.size()>0) {
        emit error(errorMsg);
        return;
    }

    if (clusters == mCurrentClusterList)
        return;    
        
    emit processStarted();
    metaObject()->invokeMethod(this, "doProcessDecode", Qt::QueuedConnection, Q_ARG(QString, filename), Q_ARG(QList<int>, clusters));
    mImage = QImage();
    emit imageChanged("");
}

void SalvObjectController::doProcessDecode(QString filename, const QList<int> &clusterList)
{
    delete mController;
    mController = nullptr;

    QFile f(filename);
    if (!f.exists()) {
        emit error(QString("%1: file doesn't exist").arg(filename));
        return;
    }    
    
    mController = new Controller(this);
    Jpeg::PicoJpegDecodr *decodr = new Jpeg::PicoJpegDecodr(new Jpeg::PhlegmaticChecker(mController), mController);
    LoggingCheck *check = new LoggingCheck(mController);

    mController->setEverybody(
                new GuidedFetch(filename, clusterList, mController),
                check,
                decodr
                );

    decodrInProgress();
    qApp->processEvents();

    bool success = mController->run(0);
    
    qDebug()<<"Check results: ";
    for (const auto &i : check->res()) 
        qDebug() << "\t"<<i.clusterNo<<"     "<<i.blockBegin<<"~"<<i.blockEnd;
        

    decodrAtEnd(success, check->res(), decodr->image());
    emit processEnd();
    
    int lastBlock = check->res().size()>0 ? check->res().back().blockEnd : 0;    
    qDebug()<<"Image size="<<mImage.size()<<" --- "<<(mImage.size()/8);
    if (mImage.width()>0)
        qDebug()<<"lastBlock="<<(lastBlock%(mImage.width()/8))<<","<<(lastBlock/(mImage.width()/8));
    
}

QList<int> SalvObjectController::translateClusterList(const QString &clusterList, QString &errorMsg) const
{
    bool parseOk = true;
    bool toIntOk, toIntOk2;
    QList<int> clusters;
    for (QString word: clusterList.split(QRegExp("\\s+"), QString::SkipEmptyParts) ) {
        if (word.count("-") == 1) {
            QStringList range = word.split("-", QString::SkipEmptyParts);
            if (range.size() != 2) {
                parseOk = false;
                break;
            }
            int from = range[0].toInt(&toIntOk, 16);
            int to = range[1].toInt(&toIntOk2, 16);
            if (from > to || !toIntOk || !toIntOk2) {
                parseOk = false;
                break;
            }
            for (int i=from; i<=to; ++i)
                clusters.push_back(i);
        
        } else {
            int i=word.toInt(&toIntOk, 16);
            if (!toIntOk) {
                parseOk = false;
                break;
            }
            clusters.push_back(i);
        }
    }
    
    if (parseOk)
        return clusters;
        
    errorMsg = QString("Failed to parse clusterList: %1").arg(clusterList);
    return QList<int>();
}
