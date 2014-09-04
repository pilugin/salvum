HEADERS += \
    $$PWD/icheck.h \
    $$PWD/imagecursor.h \
    $$PWD/imagehelpers.h \
    $$PWD/advancedchecker.h \
    $$PWD/picojpegdecodr.h
    
SOURCES += \
    $$PWD/private/imagecursor.cpp \
    $$PWD/private/imagehelpers.cpp \
    $$PWD/private/advancedchecker.cpp \
    $$PWD/private/picojpegdecodr.cpp
    
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH += $$PWD/.. $$PWD/../..
