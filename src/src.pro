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
    if/icheck.h \
    jpeg/imagecursor.h \
    jpeg/picojpegdecodr.h \
    jpeg/advancedchecker.h \
    util/singleton.h \
    util/ipc.h \
    util/private/ipc_private.h \
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
    ipcfetch/feedbackresults.h

SOURCES += \
    jpeg/advancedchecker.cpp \
    jpeg/picojpegdecodr.cpp \
    jpeg/imagecursor.cpp \
    filelogger.cpp \
    simplelogger.cpp \
    defaultsettings.cpp \
    rangefileresults.cpp \
    decodectrl.cpp \
    devicemapfetch.cpp \
    util/private/ipc_private.cpp \
    ipcfetch/broadcast.cpp \
    ipcfetch/recieverfetch.cpp \
    ipcfetch/feedbackresults.cpp

