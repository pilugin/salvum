TARGET = salvum-bcast

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv -lrt

include(../../src/dbus/dbus.pri)
include(../../src/common/colorgcc.pri)

SOURCES += \
    bcastdbusctrl.cpp \
    bcast.cpp \
    bcastthread.cpp \
    main.cpp 

HEADERS += \
    bcastdbusctrl.h \
    bcast.h \
    bcastthread.h
