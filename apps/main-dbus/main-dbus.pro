TARGET = salvum-main
TEMPLATE = app

QT += declarative

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../src ../..
DEPENDPATH += ../../src ../..
LIBS += -L../../lib -lsalv

include(../../src/dbus/dbus.pri)
include(../../src/ui/ui.pri)
include(../../src/common/colorgcc.pri)      

SOURCES += \
    main.cpp \
    supervisor.cpp \
    decodrdbushub.cpp \
    decoderdbuscontroller.cpp \
    bcastdbuscontroller.cpp \
    models/jpegheadsmodel.cpp \
    models/customclustersmodel.cpp \
    models/selectedheadsmodel.cpp \
    models/workspacemodel.cpp

HEADERS += \
    supervisor.h \
    supervisor_p.h \
    decodrdbushub.h \
    decoderdbuscontroller.h \
    bcastdbuscontroller.h \
    models/jpegheadsmodel.h \
    models/customclustersmodel.h \
    models/selectedheadsmodel.h \
    models/workspacemodel.h 

OTHER_FILES += \
    qml/Main.qml 

RESOURCES += \
    resources.qrc

