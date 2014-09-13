HEADERS += \
    $$PWD/types.h
    
SOURCES += \
    $$PWD/types.cpp
    
QMAKE_CXXFLAGS *= -std=c++11
INCLUDEPATH *= $$PWD/..       