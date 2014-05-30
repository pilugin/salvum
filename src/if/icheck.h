#ifndef ICHECK_H
#define ICHECK_H

class QImage;

class ICheck
{
public:
    virtual ~ICheck() {}

    virtual bool check(const QImage &image, int blockBegin, int blockEnd) =0;
};


#endif // ICHECK_H
