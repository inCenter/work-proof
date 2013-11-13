#include "Configuration.h"

Configuration::Configuration()
{
}


QVariantMap Configuration::toMap()
{
    QVariantMap request;
    QVariantMap header;
    QVariantMap configurationHeader;
    QVariantMap getCurrentStats;

    // header

    header.insert("sessionToken", SessionToken());
    header.insert("mac",          Mac());
    header.insert("timestamp",    Timestamp());
    header.insert("timezone",     Timezone());

    getCurrentStats.insert("getCurrentStats", 1);
    configurationHeader.insert("update", getCurrentStats);

    request.insert("header", header);
    request.insert("configuration", configurationHeader);

    return request;
}
