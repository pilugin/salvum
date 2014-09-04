#include <dbus/declareDBusMetatypes.h>
#include <QtDBus>
#include <QtDebug>

static struct DBusMetatypesRegistrer
{
    DBusMetatypesRegistrer()
    {
        qRegisterMetaType<Pixmap>("Pixmap");
        qRegisterMetaType<DecodedClusterInfo>("DecodedClusterInfo");
        qRegisterMetaType<DecodedClusters>("DecodedClusters");

        qDBusRegisterMetaType<Pixmap>();
        qDBusRegisterMetaType<DecodedClusterInfo>();
        qDBusRegisterMetaType<DecodedClusters>();
    }
} theDBusMetatypesRegistrer;

///////////////// << & >>

const QDBusArgument &operator>>(const QDBusArgument &stream, DecodedClusterInfo &dc)
{
    stream.beginStructure();
    stream >> dc.clusterNo >> dc.blockBegin >> dc.blockEnd;        
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Pixmap &pixmap)
{
    stream.beginStructure();
    stream >> pixmap.width >> pixmap.height >> pixmap.pixels;
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const DecodedClusterInfo &dc)
{
    stream.beginStructure();
    stream << dc.clusterNo << dc.blockBegin << dc.blockEnd;        
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Pixmap &pixmap)
{
    stream.beginStructure();
    stream << pixmap.width << pixmap.height << pixmap.pixels;
    stream.endStructure();
    return stream;
}
