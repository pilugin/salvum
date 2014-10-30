#include <core/controller.h>
#include <core/check.h>
#include <core/fetch.h>
#include <core/decodr.h>
#include <core/result.h>

#include <util/ilog.h>

using namespace Log;

namespace Core {

Controller::Controller(QObject *parent)
    : QObject(parent),
      mFetch(nullptr),
      mCheck(nullptr),
      mDecodr(nullptr)
{

}

void Controller::setEverybody(Fetch *fetch, Check *check, Decodr *decodr)
{
    Q_ASSERT(fetch && check && decodr);

    mFetch = fetch;
    mCheck = check;
    mDecodr = decodr;

    connect(mFetch,     SIGNAL(fetched(int,QByteArray)),    mCheck,     SLOT(onFetch(int,QByteArray))   );
    connect(mFetch,     SIGNAL(end()),                      mCheck,     SLOT(onFetchEnd())              );
    connect(mFetch,     SIGNAL(end()),                      this,       SLOT(fetchEnd())                );

    connect(mDecodr,    SIGNAL(accepted(DecodrFrame)),      mCheck,     SLOT(onAccept(DecodrFrame))     );
    connect(mDecodr,    SIGNAL(accepted(DecodrFrame)),      this,       SLOT(decodrAccepted())          );
    connect(mDecodr,    SIGNAL(rejected(DecodrFrame)),      mCheck,     SLOT(onReject(DecodrFrame))     );
    connect(mDecodr,    SIGNAL(done()),                     this,       SLOT(decodrDone())              );
    connect(mDecodr,    SIGNAL(done()),                     mCheck,     SLOT(onDecodrEnd())             );

    connect(mCheck,     SIGNAL(skipClusters(int,int)),      mFetch,     SLOT(skip(int,int))             );
    connect(mCheck,     SIGNAL(baselineFrame(DecodrFrame)), mDecodr,    SLOT(loadFrame(DecodrFrame))    );    
}

void Controller::addResult(Result *result)
{
    Q_ASSERT(result && mCheck);

    connect(mCheck,     SIGNAL(saveResults(Common::Clusters)),
            result,     SLOT(addClusters(Common::Clusters)) );
    connect(this,       SIGNAL(end(bool)),
            result,     SLOT(finalize(bool))            );
}

bool Controller::run(int clusterNo)
{
    mSuccess = false;

    if (! mFetch->rewind(clusterNo)) {
        Msg("ERR: fetch.rewind(%08X) failed\n", clusterNo);
        emit end(false);
        return success();
    }

    mDecodrAccepted = false;

    if (! mDecodr->restart(mFetch)) {
        Msg("ERR: decodr.restart() failed\n");
        emit end(false);
        return success();
    }

    mRunning = true;
    while (mRunning) {
        qDebug("GO!");

        mDecodr->resume();
        if (!mFetch->rewind())
            mRunning = false;

    }

    qDebug("END !!!!! %s", success() ? "OK" : "FAIL");
    emit end(success());
    return success();
}

void Controller::decodrDone()
{
    mRunning = false;
    mSuccess = true;
}

void Controller::fetchEnd()
{
    qDebug("CTRL__FETCH_END");
    if (mDecodrAccepted) { // can resume


    } else { // exit    
        mRunning = false;
        mSuccess = false;

    }

    mDecodrAccepted = false;
}

void Controller::decodrAccepted()
{
    mDecodrAccepted = true;
}

}

