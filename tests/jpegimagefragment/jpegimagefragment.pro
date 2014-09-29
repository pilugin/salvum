TEMPLATE = app

INCLUDEPATH += ../../src
DEPENDPATH += ../../src

LIBS += -L$$OUT_PWD/../../lib -lsalv -lrt

QMAKE_CXXFLAGS += -std=c++0x

QT += gui

SOURCES = main.cpp

CONFIG += debug