TEMPLATE = app

INCLUDEPATH += ../../../src
DEPENDPATH += ../../../src

LIBS += -L ../../../src -lsalv -lrt

CONFIG += debug

QMAKE_CXXFLAGS += -std=c++0x

HEADERS = createtestmedia.h \
             decoder.h

SOURCES = main.cpp \
             createtestmedia.cpp \
             decoder.cpp
