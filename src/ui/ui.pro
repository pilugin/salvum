include(ui.pri)
include(../dbus/dbus.pri)

TEMPLATE = lib
CONFIG += qt plugin
TARGET = salv-ui

DESTDIR = $$OUT_PWD/..

OTHER_FILES += $$PWD/qmldir