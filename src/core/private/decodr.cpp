#include <core/decodr.h>

static struct DecodrFrame_Register
{
    DecodrFrame_Register()
    {
        qRegisterMetaType<Core::DecodrFrame>("Core::DecodrFrame");
    }
} the_DecodrFrame_Register;

namespace Core {

Decodr::Decodr(QObject *parent) : QObject(parent)
{
}

///////////////////////////////////////////////////////

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

}

