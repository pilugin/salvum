QT += dbus

HEADERS += $$PWD/declareDBusMetatypes.h
SOURCES += $$PWD/declareDBusMetatypes.cpp

INCLUDEPATH+= $$OUT_PWD $$PWD

DBUSFILES=$$PWD/org.salvum.Broadcast.xml

genproxies.input = DBUSFILES
genproxies.commands = qdbusxml2cpp ${QMAKE_FILE_NAME} -p ${QMAKE_FILE_BASE} -i "declareDBusMetatypes.h"
genproxies.output = ${QMAKE_FILE_BASE}.h
genproxies.variable_out = HEADERS

DBUSFILES_2=$$DBUSFILES
genproxies_cpp.input = DBUSFILES_2
genproxies_cpp.commands = echo "dbusxml2cpp-dummy:" ${QMAKE_FILE_BASE}.cpp
genproxies_cpp.output = ${QMAKE_FILE_BASE}.cpp
genproxies_cpp.variable_out = SOURCES
genproxies_cpp.depends = ${QMAKE_FILE_BASE}.h


QMAKE_EXTRA_COMPILERS += genproxies genproxies_cpp

