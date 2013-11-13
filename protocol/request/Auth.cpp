#include "Auth.h"

Auth::Auth()
{
}

QVariantMap Auth::toMap()
{
    QVariantMap request;
    QVariantMap authHeader;

    authHeader.insert("licenseCode",        LicenseCode());
    authHeader.insert("workgroup",          Workgroup());
    authHeader.insert("userName",           Username());
    authHeader.insert("mac",                Mac());
    authHeader.insert("timezoneUTCOffset",  Timezone());

    request.insert("auth", authHeader);

    return request;
}
