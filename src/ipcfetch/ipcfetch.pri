HEADERS += \
    $$PWD/common.h \
    $$PWD/broadcast.h \
    $$PWD/recieverfetch.h
    
SOURCES += \
    $$PWD/private/broadcast.cpp \
    $$PWD/private/recieverfetch.cpp

QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH *= $$PWD/..