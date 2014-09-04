HEADERS += \
    $$PWD/array.h \
    $$PWD/ipc.h \
#    $$PWD/private/ipc_private.h \
#    $$PWD/private/container.h \    
    $$PWD/singleton.h \
    $$PWD/slotclosure.h \
    $$PWD/ilog.h \
    $$PWD/isettings.h \
    $$PWD/defaultsettings.h \
    $$PWD/simplelogger.h \
    $$PWD/filelogger.h \
    $$PWD/rangefileresults.h \
    $$PWD/devicemapfetch.h
    
SOURCES += \
    $$PWD/private/slotclosure.cpp \
    $$PWD/private/ipc_private.cpp \
#    $$PWD/private/synchromem.cpp \
    $$PWD/private/defaultsettings.cpp \
    $$PWD/private/devicemapfetch.cpp \
    $$PWD/private/simplelogger.cpp \
    $$PWD/private/filelogger.cpp \
    $$PWD/private/rangefileresults.cpp
    
QMAKE_CXXFLAGS *= -std=c++11
    
INCLUDEPATH += $$PWD/..    
