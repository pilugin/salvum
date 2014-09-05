QT       += core gui

TARGET = decodutil
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L$$OUT_PWD/../../lib -lsalv

SOURCES += main.cpp
