#include "salvobjectcontroller.h"
#include "guidedfetch.h"
#include "loggingcheck.h"

#include <core/controller.h>
#include <util/simplecheck.h>
#include <jpeg/phlegmaticchecker.h>
#include <jpeg/picojpegdecodr.h>
#include <jpeg/advancedchecker.h>

#include <QtCore>

SalvObjectController::SalvObjectController(QObject *parent) :
    SalvJpegObject(1, "salv", parent),
    mController(nullptr)
{
}

void SalvObjectController::processDecode(QString filename, QString clusterList)
{
    delete mController;
    mController = nullptr;

    QFile f(filename);
    if (!f.exists()) {
        emit error(QString("%1: file doesn't exist").arg(filename));
        return;
    }

    QList<int> clusters;
    foreach (QString c, clusterList.split(" ", QString::SkipEmptyParts)) {
        bool ok;
        clusters.push_back(c.toInt(&ok, 16));
        if (!ok) {
            emit error(QString("Failed to parse clusterList: %1").arg(c));
            return;
        }
    }

    mController = new Controller(this);
    Jpeg::PicoJpegDecodr *decodr = new Jpeg::PicoJpegDecodr(new Jpeg::PhlegmaticChecker(mController), mController);


    mController->setEverybody(
                new GuidedFetch(filename, clusters, mController),
                new SimpleCheck(true, mController),
                decodr
                );

    decodrInProgress();
    emit processStarted();
    qApp->processEvents();

    bool success = mController->run(0);


    DecodedClusters decodedClusters;
    decodrAtEnd(success, decodedClusters, decodr->image());
    emit processEnd();
}
