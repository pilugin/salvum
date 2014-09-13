HEADERS += \
    $$PWD/common.h \
    $$PWD/reader.h \
    $$PWD/writer.h
    
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH *= $$PWD/..    
DEPENDPATH *= $$PWD/../util