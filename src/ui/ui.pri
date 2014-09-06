HEADERS += \
    $$PWD/decodedclustersmodel.h \
    $$PWD/salvjpegobject.h
    
SOURCES += \
    $$PWD/decodedclustersmodel.cpp \
    $$PWD/salvjpegobject.cpp
    
QMAKE_CXXFLAGS *= -std=c++11    

QML_FILES += \
    $$PWD/qml/DecodedClustersView.qml
    
OTHER_FILES += $$QML_FILES

INCLUDEPATH *= $$PWD/.. $$PWD/../..
