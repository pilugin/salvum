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

    connect(mFetch,     SIGNAL(fetched(int)),               mCheck,     SLOT(onFetch(int))              );
    connect(mFetch,     SIGNAL(end()),                      mCheck,     SLOT(onFetchEnd())              );
    connect(mFetch,     SIGNAL(end()),                      this,       SLOT(fetchEnd())                );

    connect(mDecodr,    SIGNAL(accepted(Core::DecodrFrame)),mCheck,     SLOT(onAccept(Core::DecodrFrame))   );
    connect(mDecodr,    SIGNAL(accepted(Core::DecodrFrame)),this,       SLOT(decodrAccepted())          );
    connect(mDecodr,    SIGNAL(rejected(Core::DecodrFrame)),mCheck,     SLOT(onReject(Core::DecodrFrame))   );
    connect(mDecodr,    SIGNAL(done()),                     this,       SLOT(decodrDone())              );
    connect(mDecodr,    SIGNAL(done()),                     mCheck,     SLOT(onFetchEnd())              );

    connect(mCheck,     SIGNAL(skipClusters(int,int)),      mFetch,     SLOT(skip(int,int))             );
    connect(mCheck,     SIGNAL(baselineFrame(Core::DecodrFrame)), mDecodr,    SLOT(loadFrame(Core::DecodrFrame))    );
}

void Controller::addResult(Result *result)
{
    Q_ASSERT(result && mCheck);

    connect(mCheck,     SIGNAL(skipClusters(int,int)),      result,     SLOT(addClusters(int,int))      );
    connect(this,       SIGNAL(end(bool)),                  result,     SLOT(finalize(bool))            );
}

bool Controller::run(int clusterNo)
{
    mSuccess = false;

    if (! mFetch->rewind(clusterNo)) {
        Msg("ERR: fetch.rewind(%08X) failed\n", clusterNo);
        return success();
    }

    mDecodrAccepted = false;

    if (! mDecodr->restart(mFetch)) {
        Msg("ERR: decodr.restart() failed\n");
        return success();
    }

    mRunning = true;
    while (mRunning) {
        qDebug("GO!");

        mDecodr->resume();
        if (!mFetch->rewind())
            mRunning = false;

    }

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

