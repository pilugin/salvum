include(core-3.pri)

TEMPLATE = lib
CONFIG += staticlib
TARGET = salv-core-3

DESTDIR = $$OUT_PWD/..

SOURCES += $$PWD/compiletest.cpp

include(../common/colorgcc.pri)
