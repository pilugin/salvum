QT += dbus

HEADERS += $$PWD/declareDBusMetatypes.h \
	    $$PWD/dbustypes.h
SOURCES += $$PWD/declareDBusMetatypes.cpp

DBUSFILES = $$PWD/org.salvum.Broadcast.xml \
	    $$PWD/org.salvum.DecodrCtrl.xml

INCLUDEPATH+= $$OUT_PWD $$PWD


genproxies.input = DBUSFILES
genproxies.commands = qdbusxml2cpp ${QMAKE_FILE_NAME} -p ${QMAKE_FILE_BASE} -i "declareDBusMetatypes.h"
genproxies.output = ${QMAKE_FILE_BASE}.h
genproxies.variable_out = HEADERS

genproxies_cpp.input = DBUSFILES
genproxies_cpp.commands = echo "dbusxml2cpp-dummy:" ${QMAKE_FILE_BASE}.cpp
genproxies_cpp.output = ${QMAKE_FILE_BASE}.cpp
genproxies_cpp.variable_out = SOURCES
genproxies_cpp.depends = ${QMAKE_FILE_BASE}.h


QMAKE_EXTRA_COMPILERS += genproxies genproxies_cpp

OTHER_FILES = $$DBUSFILES
