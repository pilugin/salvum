#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T18:19:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = devicemapfetch
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src
DEPENDPATH += ../../src
LIBS += -L../../src -lsalv
#-L../../picojpeg -lpicojpeg

SOURCES += main.cpp
