#ifndef SALVOBJECTCONTROLLER_H
#define SALVOBJECTCONTROLLER_H

#include <ui/salvjpegobject.h>

class Controller;

class SalvObjectController : public SalvJpegObject
{
    Q_OBJECT
public:
    explicit SalvObjectController(QObject *parent = 0);

signals:
    void error(QString message);
    void processStarted();
    void processEnd();

public slots:
    void processDecode(QString filename, QString clusterList);
private slots:
    void doProcessDecode(QString filename, QString clusterList);    

private:
    Controller *mController;
};

#endif // SALVOBJECTCONTROLLER_H
