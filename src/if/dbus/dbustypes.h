#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QtCore/QVector>
#include <tuple>

typedef QVector< std::tuple<int /*clusterNo*/, int /*pixmapBlockBegin*/, int /*pixmapBlockCount*/> > DecodedClusters;
typedef std::tuple< int /*width*/, int /*height*/, QVector<int> /*pixels*/ > Pixmap;

#endif