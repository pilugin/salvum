#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QtCore/QVector>
#include <tuple>
#include <QMetaType>

typedef QVector< std::tuple<int /*clusterNo*/, int /*pixmapBlockBegin*/, int /*pixmapBlockCount*/> > DecodedClusters;
typedef std::tuple< int /*width*/, int /*height*/, QVector<int> /*pixels*/ > Pixmap;

Q_DECLARE_METATYPE(QVector<int>)
Q_DECLARE_METATYPE(DecodedClusters)
Q_DECLARE_METATYPE(Pixmap)


#endif