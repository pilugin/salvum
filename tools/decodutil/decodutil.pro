include(../../src/ui/ui.pri)
include(../../src/dbus/dbus.pri)
include(../../src/core/core.pri)
include(../../src/util/util.pri)
include(../../src/jpeg/jpeg.pri)

QT       += declarative

TARGET = decodutil
TEMPLATE = app

LIBS += -L$$OUT_PWD/../../lib -lsalv

SOURCES += main.cpp \
    salvobjectcontroller.cpp \
    loggingcheck.cpp \
    salvobjectimageprovider.cpp

OTHER_FILES += \
    qml/Main.qml

HEADERS += \
    salvobjectcontroller.h \
    loggingcheck.h \
    salvobjectimageprovider.h

include(../../src/common/colorgcc.pri)

CONFIG += debug