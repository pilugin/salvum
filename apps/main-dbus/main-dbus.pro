TARGET = salvum-main

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../src -lsalv

include(../../src/if/dbus/dbus.pri)

SOURCES += \
    main.cpp \
    decodrdbushub.cpp \
    decodrdbusctrl.cpp

HEADERS += \
    decodrdbushub.h \
    decodrdbusctrl.h

