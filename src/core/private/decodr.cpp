#include <core/decodr.h>

Decodr::Decodr(QObject *parent) : QObject(parent)
{
}

///////////////////////////////////////////////////////

static struct DecodrFrame_Register
{
    DecodrFrame_Register()
    {
        qRegisterMetaType<DecodrFrame>("DecodrFrame");
    }
} the_DecodrFrame_Register;


DecodrFrame::DecodrFrame(int id_, int type)
{
    mId = id_;
    mType = type;
}

DecodrFrame::~DecodrFrame()
{
}

DecodrFrame *DecodrFrame::clone() const
{
    return new DecodrFrame(id(), type());
}
