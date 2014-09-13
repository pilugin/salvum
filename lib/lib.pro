TEMPLATE = lib
TARGET = salv

LIBS += -L$$OUT_PWD/../picojpeg -lpicojpeg 

include(../src/common/common.pri)
include(../src/core/core.pri)
include(../src/dbus/dbus.pri)
include(../src/rdwr/rdwr.pri)
include(../src/util/util.pri)
include(../src/jpeg/jpeg.pri)
include(../src/ipcfetch/ipcfetch.pri)
include(../src/ui/ui.pri)

include(../src/common/colorgcc.pri)
