QT       += core gui

TARGET = salv
CONFIG   += console debug debug
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L$$OUT_PWD/../../lib -lsalv

include(../../src/common/colorgcc.pri)

SOURCES += main.cpp
