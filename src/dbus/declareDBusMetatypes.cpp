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
        qRegisterMetaType<Result>("Result");
        qRegisterMetaType<BitmapInfo>("BitmapInfo");

        qDBusRegisterMetaType<Pixmap>();
        qDBusRegisterMetaType<DecodedClusterInfo>();
        qDBusRegisterMetaType<DecodedClusters>();
        qDBusRegisterMetaType<Result>();
        qDBusRegisterMetaType<BitmapInfo>();
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

const QDBusArgument &operator>>(const QDBusArgument &stream, Result &result)
{
    stream.beginStructure();
    stream >> result.errorCode >> result.error;
    stream.endStructure();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, BitmapInfo &info)
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

QDBusArgument &operator<<(QDBusArgument &stream, const Result &result)
{  
    stream.beginStructure();
    stream << result.errorCode << result.error;
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const BitmapInfo &info)
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

