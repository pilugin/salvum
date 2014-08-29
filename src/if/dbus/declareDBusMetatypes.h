#ifndef DECLARE_DBUS_METATYPES_H
#define DECLARE_DBUS_METATYPES_H

#include <QtCore/QMetaType>
#include "dbustypes.h"

//Q_DECLARE_METATYPE(QVector<int>)
//Q_DECLARE_METATYPE(DecodedClusters)
//Q_DECLARE_METATYPE(Pixmap)

#include <QDBusArgument>

const QDBusArgument &operator>>(const QDBusArgument &stream, DecodedClusters &decodedClusters);
const QDBusArgument &operator>>(const QDBusArgument &stream, Pixmap &pixmap);
QDBusArgument &operator<<(QDBusArgument &stream, const DecodedClusters &decodedClusters);
QDBusArgument &operator<<(QDBusArgument &stream, const Pixmap &pixmap);



#endif