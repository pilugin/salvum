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




genadaptors.input = DBUSFILES
genadaptors.commands = qdbusxml2cpp ${QMAKE_FILE_NAME} -a ${QMAKE_FILE_BASE}Adp -i "declareDBusMetatypes.h"
genadaptors.output = ${QMAKE_FILE_BASE}Adp.h
genadaptors.variable_out = HEADERS

genadaptors_cpp.input = DBUSFILES
genadaptors_cpp.commands = echo "dbusxml2cpp-adp-dummy:" ${QMAKE_FILE_BASE}Adp.cpp
genadaptors_cpp.output = ${QMAKE_FILE_BASE}Adp.cpp
genadaptors_cpp.variable_out = SOURCES
genadaptors_cpp.depends = ${QMAKE_FILE_BASE}Adp.h




QMAKE_EXTRA_COMPILERS += genproxies genproxies_cpp   genadaptors genadaptors_cpp

OTHER_FILES = $$DBUSFILES
