QT       += core

TARGET = bcastctrl
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../src -lsalv -lrt
#-L../../picojpeg -lpicojpeg

SOURCES += \
    main.cpp \
   bcastremotectrl.cpp

HEADERS += \
    bcastremotectrl.h
