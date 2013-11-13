#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QVector>
#include <QVariantMap>

#include "protocol/request/Header.h"

class Configuration : public Header
{
public:
    Configuration();

    QVariantMap toMap();
};

#endif // CONFIGURATION_H
