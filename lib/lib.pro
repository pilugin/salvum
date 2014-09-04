TEMPLATE = lib
TARGET = salv

LIBS += -L$$OUT_PWD/../picojpeg -lpicojpeg 
#LIBS += -L$$OUT_PWD/../src/ -lsalv-core -lsalv-dbus -lsalv-rdwr -lsalv-ipcfetch -lsalv-util -lsalv-jpeg

include(../src/core/core.pri)
include(../src/dbus/dbus.pri)
include(../src/rdwr/rdwr.pri)
include(../src/util/util.pri)
include(../src/jpeg/jpeg.pri)
include(../src/ipcfetch/ipcfetch.pri)