#include "util/slotclosure.h"

SlotClosure::SlotClosure(QObject *object, const char *slot) :
    mObject(slot ? object : 0)
{
    if (mObject) {
        int index = mObject->metaObject()->/*indexOfSlot*/indexOfMethod(++slot);
        if (index == -1) {
            qDebug("Cannot find slot '%s'", slot);
        } else {
            mSlot = mObject->metaObject()->method(index);
        }
    }
}

bool SlotClosure::operator==(const SlotClosure &other) const
{
    return mObject == other.mObject
#if QT_VERSION >= 0x050000
        && mSlot.methodSignature() == other.mSlot.methodSignature()
#else
        && mSlot.signature() == other.mSlot.signature()
#endif
    ;
}

void SlotClosure::call(QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9)
{
    if (mObject)
        mSlot.invoke(mObject, Qt::QueuedConnection, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
}

////////////////////

SlotBindingClosure::SlotBindingClosure(QObject *object, const char *slot, QGenericArgument val0, QGenericArgument val1, 
       QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, 
       QGenericArgument val7, QGenericArgument val8, QGenericArgument val9)
: SlotClosure(object, slot)
{
    mArgs   <<val0<<val1<<val2<<val3<<val4
            <<val5<<val6<<val7<<val8<<val9;
}       

void SlotBindingClosure::bindingCall()
{
    call(   mArgs[0], mArgs[1], mArgs[2], mArgs[3], mArgs[4],
            mArgs[5], mArgs[6], mArgs[7], mArgs[8], mArgs[9]);
}
