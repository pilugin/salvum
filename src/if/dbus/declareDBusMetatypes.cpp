#include "declareDBusMetatypes.h"

class DBusMetatypesRegistrer
{
public:
    DBusMetatypesRegistrer()
    {
	qRegisterMetaType<QVector<int> >();
    }
};

static DBusMetatypesRegistrer theDBusMetatypesRegistrer;