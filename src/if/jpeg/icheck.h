#ifndef ICHECK_H
#define ICHECK_H

/// used by jpeg decoder only

class QImage;

namespace Jpeg 
{

class ICheck
{
public:
    virtual ~ICheck() {}

    virtual bool check(const QImage &image, int blockBegin, int blockEnd) =0;
};

} // eons Jpeg

#endif // ICHECK_H
