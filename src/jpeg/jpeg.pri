HEADERS += \
    $$PWD/icheck.h \
    $$PWD/imagecursor.h \
    $$PWD/imagehelpers.h \
    $$PWD/advancedchecker.h \
    $$PWD/picojpegdecodr.h \
    $$PWD/phlegmaticchecker.h \
    $$PWD/algo.h \
    $$PWD/thumbnailcreator.h
    
SOURCES += \
    $$PWD/private/imagecursor.cpp \
    $$PWD/private/imagehelpers.cpp \
    $$PWD/private/advancedchecker.cpp \
    $$PWD/private/picojpegdecodr.cpp \
    $$PWD/private/algo.cpp \
    $$PWD/private/thumbnailcreator.cpp
    
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH *= $$PWD/.. $$PWD/../..
DEPENDPATH *= $$PWD/../common $$PWD/../util $$PWD/../core