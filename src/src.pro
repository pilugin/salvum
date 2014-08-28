TEMPLATE = lib
TARGET = salv
DEPENDPATH += . if util ipc rdwr ipcfetch
INCLUDEPATH += . ..
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++0x

LIBS += -L../picojpeg -lpicojpeg

HEADERS += \
    if/ifetch.h \
    if/ilog.h \
    if/idecod.h \
    if/iresults.h \
    if/jpeg/icheck.h \
    jpeg/imagecursor.h \
    jpeg/picojpegdecodr.h \
    jpeg/advancedchecker.h \
    jpeg/imagehelpers.h \
    util/singleton.h \
    util/ipc.h \
    util/private/ipc_private.h \
    util/slotclosure.h \
    rdwr/common.h \
    rdwr/reader.h \
    rdwr/writer.h \
    filelogger.h \
    simplelogger.h \
    defaultsettings.h \
    decodectrl.h \
    devicemapfetch.h \
    rangefileresults.h \
    ipcfetch/common.h \
    ipcfetch/broadcast.h \
    ipcfetch/recieverfetch.h \
    ipcfetch/feedbackresults.h \
    ipcfetch/bcastctrlcommon.h \
    ipcfetch/bcastctrlagent.h

SOURCES += \
    jpeg/advancedchecker.cpp \
    jpeg/picojpegdecodr.cpp \
    jpeg/imagecursor.cpp \
    jpeg/imagehelpers.cpp \
    filelogger.cpp \
    simplelogger.cpp \
    defaultsettings.cpp \
    rangefileresults.cpp \
    decodectrl.cpp \
    devicemapfetch.cpp \
    util/private/ipc_private.cpp \
    util/private/slotclosure.cpp \
    ipcfetch/broadcast.cpp \
    ipcfetch/recieverfetch.cpp \
    ipcfetch/feedbackresults.cpp \
    ipcfetch/bcastctrlagent.cpp

