TARGET = salvum-decodr

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv -lrt

include(../../src/dbus/dbus.pri)
include(../../src/common/colorgcc.pri)

SOURCES += \
    main.cpp \
    salvdbuscheck.cpp \
    dbusdecodeprocessor.cpp \
    eventlooprecieverfetch.cpp

HEADERS += \
    salvdbuscheck.h \
    dbusdecodeprocessor.h \
    eventlooprecieverfetch.h
    
RESOURCES +=



