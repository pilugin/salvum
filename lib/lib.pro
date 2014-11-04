TEMPLATE = lib
TARGET = salv
CONFIG += debug
LIBS += -L$$OUT_PWD/../picojpeg -lpicojpeg 

include(../src/common/common.pri)
include(../src/core-3/core-3.pri)
include(../src/dbus/dbus.pri)
include(../src/rdwr/rdwr.pri)
include(../src/util/util.pri)
include(../src/jpeg/jpeg.pri)
include(../src/ipcfetch/ipcfetch.pri)
include(../src/ui/ui.pri)
#LIBS+= -L$$OUT_PWD/../src -lsalv-{common,core,dbus,ipcfetch,jpeg,rdwr,util}

include(../src/common/colorgcc.pri)

DEPENDPATH+=$$PWD/../src
