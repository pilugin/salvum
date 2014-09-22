TARGET = salvum-decodr

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv

include(../../src/dbus/dbus.pri)
include(../../src/common/colorgcc.pri)

SOURCES += \
    main.cpp \
    salvdbuscheck.cpp \
    dbusdecodeprocessor.cpp

HEADERS += \
    salvdbuscheck.h \
    dbusdecodeprocessor.h
    
RESOURCES +=



