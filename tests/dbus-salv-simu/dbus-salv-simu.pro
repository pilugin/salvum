#-------------------------------------------------
#
# Project created by QtCreator 2014-08-29T19:21:31
#
#-------------------------------------------------

TARGET = dbus-salv-simu
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../src ../.. $$OUT_PWD/../../src
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv

SOURCES += main.cpp

QT += dbus

