TARGET = salvum-main
TEMPLATE = app

QT += declarative

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv

include(../../src/dbus/dbus.pri)
include(../../src/ui/ui.pri)

SOURCES += \
    main.cpp \
    decodrdbushub.cpp \
    decodrdbusctrl.cpp \
    salvjpegmodel.cpp \
    salvjpegimageprovider.cpp \
    bcastdbuscontroller.cpp \
    jpegheadsmodel.cpp

HEADERS += \
    decodrdbushub.h \
    decodrdbusctrl.h \
    salvjpegmodel.h \
    salvjpegimageprovider.h \
    bcastdbuscontroller.h \
    jpegheadsmodel.h

OTHER_FILES += \
    qml/SalvJpegView.qml \
    qml/Main.qml

RESOURCES += \
    resources.qrc

