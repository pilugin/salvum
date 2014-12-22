TARGET = salvum-decodr

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

QT += dbus
INCLUDEPATH += ../../src ../..     $$OUT_PWD/../../src
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv -L../../src -lsalv-dbus -lrt

#include(../../src/dbus/dbus.pri)
include(../../src/common/colorgcc.pri)

SOURCES += \
    main.cpp \
    salvdbuscheck.cpp \
    dbusdecodeprocessor.cpp \
    eventlooprecieverfetch.cpp \
    salvdbusarchive.cpp

HEADERS += \
    salvdbuscheck.h \
    dbusdecodeprocessor.h \
    eventlooprecieverfetch.h \
    salvdbusarchive.h
    
RESOURCES +=



