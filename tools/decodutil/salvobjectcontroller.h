#ifndef SALVOBJECTCONTROLLER_H
#define SALVOBJECTCONTROLLER_H

#include <ui/salvjpegobject.h>

namespace Core {
class Controller;
}

class SalvObjectController : public Ui::SalvJpegObject
{
    Q_OBJECT
public:
    explicit SalvObjectController(QObject *parent = 0);
    
    static QString imageProviderName() { return "salv"; }

signals:
    void error(QString message);
    void processStarted();
    void processEnd();

public slots:
    void processDecode(QString filename, QString clusterList);
private slots:
    void doProcessDecode(QString filename, const QList<int> &clusterList);    

private:
    QList<int> translateClusterList(const QString &clusterList, QString &errorMsg) const;

    Core::Controller *mController;
    QList<int> mCurrentClusterList;
};

#endif // SALVOBJECTCONTROLLER_H
