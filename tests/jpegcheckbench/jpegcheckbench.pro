TEMPLATE = app

INCLUDEPATH += ../../src ../../
DEPENDPATH += ../../src

LIBS += -L ../../src -lsalv -lrt 
#-L ../../picojpeg -lpicojpeg

QMAKE_CXXFLAGS += -std=c++0x

HEADERS = bench.h
SOURCES = main.cpp bench.cpp

CONFIG += debug
