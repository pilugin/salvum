TEMPLATE = app

INCLUDEPATH += ../../../src
DEPENDPATH += ../../../src

LIBS += -L ../../../src -lsalv -lrt

CONFIG += debug

QMAKE_CXXFLAGS += -std=c++0x

SOURCES = main.cpp
