HEADERS += \
    $$PWD/fetch.h \
    $$PWD/check.h \
    $$PWD/decodr.h \
    $$PWD/archive.h    
    
SOURCES += \
    $$PWD/fetch.cpp 
    
INCLUDEPATH *= $$PWD/..
    
QMAKE_CXXFLAGS *= -std=c++11        
