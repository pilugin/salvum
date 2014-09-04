#ifndef CORE_CONTROLLER_H
#define CORE_CONTROLLER_H

#include <QObject>

class Result;
class Check;
class Fetch;
class Decodr;

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject *parent =nullptr);

    void setEverybody(Fetch *fetch, Check *check, Decodr *decodr);
    void addResult(Result *result);

    void run(int clusterNo);

signals:
    void end(bool success);

private slots:
    void fetchEnd();
    void decodrAccepted();
    void decodrDone();


private:
    Fetch *mFetch;
    Check *mCheck;
    Decodr *mDecodr;

    bool mDecodrAccepted;
    bool mRunning;
};

#endif // CORE_CONTROLLER_H
