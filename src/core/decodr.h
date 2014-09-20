#ifndef CORE_DECODR_H
#define CORE_DECODR_H

#include <QObject>
#include <QMetaType>

namespace Core {

class Fetch;

class DecodrFrame
{
public:
    enum 
    { 
        EmptyType = 0,
        CustomType,
    };
    
    DecodrFrame(int id =0, int type =EmptyType);
    virtual ~DecodrFrame();
    
    virtual DecodrFrame *clone() const;

    int type() const { return mType; }
    int id() const { return mId; }
private:
    int mId;    
    int mType;
};






class Decodr : public QObject
{
    Q_OBJECT
    
public:
    Decodr(QObject *parent =nullptr);
    virtual bool isDone() const =0;

public slots:
    virtual bool restart(Fetch *fetch) =0;
    virtual void resume() =0;
    virtual void loadFrame(const DecodrFrame &frame) =0;

signals:
    void accepted(const DecodrFrame &frame);
    void rejected(const DecodrFrame &frame);
    void done();
};

}

Q_DECLARE_METATYPE(Core::DecodrFrame)


#endif // CORE_DECODR_H
