TEMPLATE = lib
TARGET = salv

LIBS += -L$$OUT_PWD/../picojpeg -lpicojpeg 
LIBS += -L$$OUT_PWD/../src/ -lsalv-core -lsalv-dbus -lsalv-rdwr -lsalv-ipcfetch -lsalv-util -lsalv-jpeg
