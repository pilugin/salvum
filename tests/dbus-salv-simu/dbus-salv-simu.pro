#-------------------------------------------------
#
# Project created by QtCreator 2014-08-29T19:21:31
#
#-------------------------------------------------

TARGET = dbus-salv-simu
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../src -lsalv

SOURCES += main.cpp

include(../../src/if/dbus/dbus.pri)

RESOURCES += \
    pics.qrc
