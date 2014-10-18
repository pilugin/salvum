#ifndef UI_RECT_H
#define UI_RECT_H

#include <QObject>

namespace Ui {

class Rect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal x      READ x      WRITE setX      NOTIFY xChanged)
    Q_PROPERTY(qreal y      READ y      WRITE setY      NOTIFY yChanged)
    Q_PROPERTY(qreal width  READ width  WRITE setWidth  NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
public:
    Rect(QObject *parent =nullptr) : QObject(parent), mX(0), mY(0), mWidth(0), mHeight(0) {}
    
    qreal x() const { return mX; }
    qreal y() const { return mY; }
    qreal width() const { return mWidth; }
    qreal height() const { return mHeight; }
    void setX(qreal x_) { mX = x_; emit xChanged(); }
    void setY(qreal y_) { mY = y_; emit yChanged(); }
    void setWidth(qreal w) { mWidth = w; emit widthChanged(); }
    void setHeight(qreal h) { mHeight = h; emit heightChanged(); }
    void reset() { setX(0); setY(0); setWidth(0); setHeight(0); }
signals:
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();    
private:
    qreal mX, mY, mWidth, mHeight;    
};

}

#endif // UI_RECT_H