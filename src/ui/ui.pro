#
# After make install it creates SalvUi module (TBD)
#

include(ui.pri)
include(../common/colorgcc.pri)

TEMPLATE = lib
CONFIG += qt plugin
TARGET = salv-ui

DESTDIR = $$OUT_PWD/..

OTHER_FILES += $$PWD/qmldir