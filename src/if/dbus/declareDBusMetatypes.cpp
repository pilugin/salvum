#include "declareDBusMetatypes.h"

class DBusMetatypesRegistrer
{
public:
    DBusMetatypesRegistrer()
    {
	qRegisterMetaType<QVector<int>>();
	qRegisterMetaType<Pixmap>();
	qRegisterMetaType<DecodedClusters>();
	
    }
};

static DBusMetatypesRegistrer theDBusMetatypesRegistrer;