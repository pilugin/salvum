#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QObject>

class Supervisor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString qmlScreen READ qmlScreen WRITE qmlScreen NOTIFY qmlScreenChanged)
    Q_PROPERTY(QList<int> decodedHeads READ decodedHeads NOTIFY decodedHeadsChanged)
public:
    Supervisor(QObject *parent =nullptr);
    
    QString qmlScreen() const;
    void qmlScreen(const QString &value);
    QList<int> decodedHeads() const;
signals:
    void qmlScreenChanged();    
    void decodedHeadsChanged();
    
    void decodeStateEntered();
    
public slots:
    void init();

    void startDecode(const QList<int> &clusters);
    void startDecode(const QVariant &clusters);
    void allDecodersConnected();
    void broadcastAtEnd();
    
private:
    struct Private;
    Private *m_d;    
};

#endif // SUPERVISOR_H