include(jpeg.pri)

TEMPLATE = lib
CONFIG += staticlib
TARGET = salv-jpeg

DESTDIR = $$OUT_PWD/..

include(../common/colorgcc.pri)
