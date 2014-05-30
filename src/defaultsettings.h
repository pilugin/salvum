#ifndef DEFAULTSETTINGS_H
#define DEFAULTSETTINGS_H

#include "if/isettings.h"

class DefaultSettings : public ISettings
{
public:
    virtual QVariant get(int field) const;
};

#endif // DEFAULTSETTINGS_H
