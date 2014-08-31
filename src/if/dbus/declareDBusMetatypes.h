#ifndef DECLARE_DBUS_METATYPES_H
#define DECLARE_DBUS_METATYPES_H

#include "dbustypes.h"

#include <QDBusArgument>

const QDBusArgument &operator>>(const QDBusArgument &stream, DecodedClusterInfo &decodedClusters);
const QDBusArgument &operator>>(const QDBusArgument &stream, Pixmap &pixmap);
QDBusArgument &operator<<(QDBusArgument &stream, const DecodedClusterInfo &decodedClusters);
QDBusArgument &operator<<(QDBusArgument &stream, const Pixmap &pixmap);

#endif
