#ifndef DECLARE_DBUS_METATYPES_H
#define DECLARE_DBUS_METATYPES_H

#include <dbus/dbustypes.h>

#include <QDBusArgument>

const QDBusArgument &operator>>(const QDBusArgument &stream, DecodedClusterInfo &decodedClusters);
const QDBusArgument &operator>>(const QDBusArgument &stream, Pixmap &pixmap);
const QDBusArgument &operator>>(const QDBusArgument &stream, Result &result);
const QDBusArgument &operator>>(const QDBusArgument &stream, BitmapInfo &info);

QDBusArgument &operator<<(QDBusArgument &stream, const DecodedClusterInfo &decodedClusters);
QDBusArgument &operator<<(QDBusArgument &stream, const Pixmap &pixmap);
QDBusArgument &operator<<(QDBusArgument &stream, const Result &result);
QDBusArgument &operator<<(QDBusArgument &stream, const BitmapInfo &info);

#endif
