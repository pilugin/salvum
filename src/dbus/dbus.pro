include(dbus.pri)

TEMPLATE = lib
CONFIG += staticlib
TARGET = salv-dbus

DESTDIR = $$OUT_PWD/..

include(../common/colorgcc.pri)
#include(../common/common.pri)