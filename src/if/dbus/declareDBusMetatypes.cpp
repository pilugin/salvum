#include "declareDBusMetatypes.h"
#include <QtDBus>

class DBusMetatypesRegistrer
{
public:
    DBusMetatypesRegistrer()
    {
        qDBusRegisterMetaType<QVector<int>>();
        qDBusRegisterMetaType<Pixmap>();
        qDBusRegisterMetaType<DecodedClusters>();
    }
};

static DBusMetatypesRegistrer theDBusMetatypesRegistrer;

///////////////// << & >>

const QDBusArgument &operator>>(const QDBusArgument &stream, DecodedClusters &decodedClusters)
{
    stream.beginArray();
    while (!stream.atEnd()) {
        stream.beginStructure();
        DecodedClusters::value_type v;
        stream >> std::get<0>(v) >> std::get<1>(v) >> std::get<2>(v);        
	stream.endStructure();
	decodedClusters.push_back(v);
    }
    stream.endArray();
    return stream;
}

const QDBusArgument &operator>>(const QDBusArgument &stream, Pixmap &pixmap)
{
    stream.beginStructure();
    stream >> std::get<0>(pixmap) >> std::get<1>(pixmap);;
    stream.beginArray();
    while (!stream.atEnd()) {
	int pixel;
        stream >> pixel;
        std::get<2>(pixmap).push_back(pixel);
    }
    stream.endArray();
    stream.endStructure();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const DecodedClusters &decodedClusters)
{
    stream.beginArray(decodedClusters.size());
    for (int i=0; i<decodedClusters.size(); ++i) {
        stream.beginStructure();
        stream << std::get<0>(decodedClusters[i]) << std::get<1>(decodedClusters[i]) << std::get<2>(decodedClusters[i]);        
	stream.endStructure();
    }
    stream.endArray();
    return stream;
}

QDBusArgument &operator<<(QDBusArgument &stream, const Pixmap &pixmap)
{
    stream.beginStructure();
    stream << std::get<0>(pixmap) << std::get<1>(pixmap);
    stream.beginArray( std::get<2>(pixmap).size() );
    for (int i=0; i<std::get<2>(pixmap).size(); ++i) 
	stream << std::get<2>(pixmap)[i];
    stream.endArray();
    stream.endStructure();
    return stream;
}
