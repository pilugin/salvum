#ifndef DECLARE_DBUS_METATYPES_H
#define DECLARE_DBUS_METATYPES_H

#include <common/types.h>

#include <QDBusArgument>

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::DecodedClusterInfo &decodedCluster);
const QDBusArgument &operator>>(const QDBusArgument &stream, Common::ImageInfo &imageInfo);
const QDBusArgument &operator>>(const QDBusArgument &stream, Common::Result &result);
const QDBusArgument &operator>>(const QDBusArgument &stream, Common::BitmapInfo &info);

QDBusArgument &operator<<(QDBusArgument &stream, const Common::DecodedClusterInfo &decodedCluster);
QDBusArgument &operator<<(QDBusArgument &stream, const Common::ImageInfo &imageInfo);
QDBusArgument &operator<<(QDBusArgument &stream, const Common::Result &result);
QDBusArgument &operator<<(QDBusArgument &stream, const Common::BitmapInfo &info);

#endif
