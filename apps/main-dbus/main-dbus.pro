TARGET = salvum-main
TEMPLATE = app

QT += declarative

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../src -lsalv

include(../../src/if/dbus/dbus.pri)

SOURCES += \
    main.cpp \
    decodrdbushub.cpp \
    decodrdbusctrl.cpp \
    salvjpegmodel.cpp \
    salvjpegobject.cpp \
    decodedclustersmodel.cpp \
    salvjpegimageprovider.cpp 

HEADERS += \
    decodrdbushub.h \
    decodrdbusctrl.h \
    salvjpegmodel.h \
    salvjpegobject.h \
    decodedclustersmodel.h \
    salvjpegimageprovider.h

OTHER_FILES += \
    qml/SalvJpegView.qml

RESOURCES += \
    resources.qrc

