TARGET = bcast

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv

include(../../src/dbus/dbus.pri)
include(../../src/common/colorgcc.pri)

SOURCES += \
    bcastdbusctrl.cpp \
    main.cpp 

HEADERS += \
    bcastdbusctrl.h
