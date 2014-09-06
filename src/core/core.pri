HEADERS += \
    $$PWD/fetch.h \
    $$PWD/check.h \
    $$PWD/decodr.h \
    $$PWD/result.h \
    $$PWD/controller.h    
    
SOURCES += \
    $$PWD/private/fetch.cpp \
    $$PWD/private/check.cpp \
    $$PWD/private/decodr.cpp \
    $$PWD/private/result.cpp \
    $$PWD/private/controller.cpp
    
INCLUDEPATH *= $$PWD/..
    
QMAKE_CXXFLAGS *= -std=c++11        
