#ifndef SLOTCLOSURE_H
#define SLOTCLOSURE_H

#include <QObject>
#include <QVector>
#include <QMetaMethod>
#include <QMetaObject>

/// Class for deferred Qt calls.
class SlotClosure
{
public:
    SlotClosure() : mObject(0) {}
    SlotClosure(QObject *object, const char *slot);

    bool operator==(const SlotClosure &other) const;

    /// Proceeds the call with the given arguments. Invokation type is Qt::QueuedConnection
    void call(QGenericArgument val0 =QGenericArgument(), QGenericArgument val1 =QGenericArgument(), QGenericArgument val2 =QGenericArgument(), QGenericArgument val3 =QGenericArgument(), QGenericArgument val4 =QGenericArgument(), QGenericArgument val5 =QGenericArgument(), QGenericArgument val6 =QGenericArgument(), QGenericArgument val7 =QGenericArgument(), QGenericArgument val8 =QGenericArgument(), QGenericArgument val9 =QGenericArgument());
    
private:
    QObject *mObject;
    QMetaMethod mSlot;
};

class SlotBindingClosure : public SlotClosure
{
public:
    SlotBindingClosure() {}
    SlotBindingClosure(QObject *object, const char *slot, QGenericArgument val0 =QGenericArgument(), QGenericArgument val1 =QGenericArgument(), QGenericArgument val2 =QGenericArgument(), QGenericArgument val3 =QGenericArgument(), QGenericArgument val4 =QGenericArgument(), QGenericArgument val5 =QGenericArgument(), QGenericArgument val6 =QGenericArgument(), QGenericArgument val7 =QGenericArgument(), QGenericArgument val8 =QGenericArgument(), QGenericArgument val9 =QGenericArgument());

    void bindingCall();
private:
    QVector<QGenericArgument> mArgs;        
};
            
#endif // SLOTCLOSURE_H