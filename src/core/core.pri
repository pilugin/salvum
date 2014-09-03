HEADERS += \
    $$PWD/fetch.h \
    $$PWD/check.h \
    $$PWD/decodr.h \
    $$PWD/result.h 
    
SOURCES += \
    $$PWD/private/fetch.cpp \
    $$PWD/private/check.cpp \
    $$PWD/private/decodr.cpp \
    $$PWD/private/result.cpp
    
    
QMAKE_CXXFLAGS *= -std=c++11        