#include "Info.h"

SystemInfo::SystemInfo(const QString & username, const QString & workgroup, const QString & mac, const QString & timezone)
{
    UserName(username);
    Workgroup(workgroup);
    MacAddress(mac);
    Timezone(timezone);
}
