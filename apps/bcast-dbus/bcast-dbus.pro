TARGET = bcast

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../src -lsalv

include(../../src/if/dbus/dbus.pri)

SOURCES += \
    bcastdbusctrl.cpp \
    main.cpp 

HEADERS += \
    bcastdbusctrl.h