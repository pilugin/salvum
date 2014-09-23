#ifndef DBUS_DECODE_PROCESSOR_H
#define DBUS_DECODE_PROCESSOR_H

#include <QObject>

class QDBusConnection;

class DbusDecodeProcessor : public QObject
{
    Q_OBJECT
public:
    DbusDecodeProcessor(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent =nullptr);
    ~DbusDecodeProcessor();
    
signals:
    void exitApp();    
    
private slots:
    void onExit();        
    void onStart(int clusterNo, const QString &shmemPath);
    void checkDbus();
    
private:
    class Private;
    Private *m_d;        
};

#endif // DBUS_DECODE_PROCESSOR_H