HEADERS += \
    $$PWD/decodedclustersmodel.h \
    $$PWD/salvjpegobject.h \
    $$PWD/bitmapinfomodel.h
    
SOURCES += \
    $$PWD/decodedclustersmodel.cpp \
    $$PWD/salvjpegobject.cpp \
    $$PWD/bitmapinfomodel.cpp
    
QMAKE_CXXFLAGS *= -std=c++11    

QT += dbus

QML_FILES += \
    $$PWD/qml/DecodedClustersView.qml
    
OTHER_FILES += $$QML_FILES

INCLUDEPATH *= $$PWD/.. $$PWD/../..
