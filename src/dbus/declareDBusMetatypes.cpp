#include <dbus/declareDBusMetatypes.h>
#include <QtDBus>
#include <QtDebug>

static struct DBusMetatypesRegistrer
{
    DBusMetatypesRegistrer()
    {
        qRegisterMetaType<Common::Pixmap>("Common::Pixmap");
        qRegisterMetaType<Common::DecodedClusterInfo>("Common::DecodedClusterInfo");
        qRegisterMetaType<Common::DecodedClusters>("Common::DecodedClusters");
        qRegisterMetaType<Common::RejectedClusterInfo>("Common::RejectedClusterInfo");
        qRegisterMetaType<Common::RejectedClusters>("Common::RejectedClusters");
        qRegisterMetaType<Common::Result>("Common::Result");
        qRegisterMetaType<Common::BitmapInfo>("Common::BitmapInfo");

        qDBusRegisterMetaType<Common::Pixmap>();
        qDBusRegisterMetaType<Common::DecodedClusterInfo>();
        qDBusRegisterMetaType<Common::DecodedClusters>();
        qDBusRegisterMetaType<Common::RejectedClusterInfo>();
        qDBusRegisterMetaType<Common::RejectedClusters>();
        qDBusRegisterMetaType<Common::Result>();
        qDBusRegisterMetaType<Common::BitmapInfo>();
    }
} theDBusMetatypesRegistrer;

///////////////// << & >>

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::DecodedClusterInfo &dc)
{
    stream.beginStructure();
    stream >> dc.clusterNo >> dc.blockBegin >> dc.blockEnd;        
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::RejectedClusterInfo &rc)
{
    stream.beginStructure();
    stream >> rc.clusterNo >> rc.blockBegin >> rc.pixels;        
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::Pixmap &pixmap)
{
    stream.beginStructure();
    stream >> pixmap.width >> pixmap.height >> pixmap.pixels;
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::Result &result)
{
    stream.beginStructure();
    stream >> result.errorCode >> result.error;
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Common::BitmapInfo &info)
{
    stream.beginStructure();
    stream  >> info.jpegHeads
            >> info.goodHeads
            >> info.zeros
            >> info.goods
            >> info.decodable
            >> info.starts;
    stream.endStructure();
    return stream;
}

//

QDBusArgument &operator<<(QDBusArgument &stream, const Common::DecodedClusterInfo &dc)
{
    stream.beginStructure();
    stream << dc.clusterNo << dc.blockBegin << dc.blockEnd;        
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Common::RejectedClusterInfo &rc)
{
    stream.beginStructure();
    stream << rc.clusterNo << rc.blockBegin << rc.pixels;        
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Common::Pixmap &pixmap)
{
    stream.beginStructure();
    stream << pixmap.width << pixmap.height << pixmap.pixels;
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Common::Result &result)
{  
    stream.beginStructure();
    stream << result.errorCode << result.error;
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Common::BitmapInfo &info)
{
    stream.beginStructure();
    stream  << info.jpegHeads
            << info.goodHeads
            << info.zeros
            << info.goods
            << info.decodable
            << info.starts;
    stream.endStructure();
    return stream;
}

