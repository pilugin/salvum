HEADERS += \
    $$PWD/decodedclustersmodel.h \
    $$PWD/salvjpegobject.h \
    $$PWD/bitmapinfomodel.h \
    $$PWD/imageprovider.h
    
SOURCES += \
    $$PWD/decodedclustersmodel.cpp \
    $$PWD/salvjpegobject.cpp \
    $$PWD/bitmapinfomodel.cpp \
    $$PWD/imageprovider.cpp
    
QMAKE_CXXFLAGS *= -std=c++11    

QT += dbus declarative

QML_FILES += \
    $$PWD/qml/DecodedClustersView.qml
    
OTHER_FILES += $$QML_FILES

INCLUDEPATH *= $$PWD/.. $$PWD/../..
