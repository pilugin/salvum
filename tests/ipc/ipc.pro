TEMPLATE = app

INCLUDEPATH += ../../src
DEPENDPATH += ../../src

LIBS += -L ../../src -lsalv -lrt

QMAKE_CXXFLAGS += -std=c++0x

SOURCES = main.cpp
