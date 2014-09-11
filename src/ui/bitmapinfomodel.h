#ifndef UI_BITMAP_INFO_MODEL_H
#define UI_BITMAP_INFO_MODEL_H

#include <dbus/dbustypes.h>
#include <QObject>

class BitmapInfoModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int jpegHeads    READ jpegHeads  NOTIFY updated)
    Q_PROPERTY(int goodHeads    READ goodHeads  NOTIFY updated)
    Q_PROPERTY(int zeros        READ zeros      NOTIFY updated)
    Q_PROPERTY(int goods        READ goods      NOTIFY updated)
    Q_PROPERTY(int decodable    READ decodable  NOTIFY updated)
    Q_PROPERTY(int starts       READ starts     NOTIFY updated)
public:
    BitmapInfoModel(QObject *parent =nullptr);
    
    int jpegHeads() const   { return mInfo.jpegHeads; }
    int goodHeads() const   { return mInfo.goodHeads; }
    int zeros() const       { return mInfo.zeros; }
    int goods() const       { return mInfo.goods; }
    int decodable() const   { return mInfo.decodable; }
    int starts() const      { return mInfo.starts; }
    
public slots:
    void setInfo(const BitmapInfo &info);
signals:
    void updated();
    
private:    
    BitmapInfo mInfo;
};

#endif // UI_BITMAP_INFO_MODEL_H