#include "Monitor.h"

SystemMonitor::SystemMonitor()
{
}


SystemInfo * SystemMonitor::getInfo()
{
    auto sysInfo = new SystemInfo();

    sysInfo->UserName( _getUsername() );
    sysInfo->Workgroup( _getWorkgroup() );
    sysInfo->MacAddress( _getMac() );
    sysInfo->Timezone( _getTimezone() );

    return sysInfo;
}


QString SystemMonitor::_getUsername()
{
    WCHAR wUserName[257]      = {0};
    DWORD nUserName = sizeof(wUserName);
    GetUserName(wUserName, &nUserName);

    return QString::fromWCharArray(wUserName);
}


QString SystemMonitor::_getWorkgroup()
{
    DWORD dwLevel = 102;
    LPWKSTA_INFO_102 pBuf = NULL;
    LPWSTR pszServerName = NULL;

    NetWkstaGetInfo(pszServerName,
                          dwLevel,
                          (LPBYTE *)&pBuf);

    return QString::fromWCharArray(pBuf->wki102_langroup);
}


QString SystemMonitor::_getMac()
{
    QString address = "11-11-11-11-11-11";

    return address;
}


QString SystemMonitor::_getTimezone()
{
    LPTIME_ZONE_INFORMATION timeZoneInfo = new TIME_ZONE_INFORMATION;
    GetTimeZoneInformation(timeZoneInfo);
    return QString::number(timeZoneInfo->Bias - timeZoneInfo->DaylightBias);
}


qint32 SystemMonitor::getLastInputSeconds()
{
    bool apiResult;
    LASTINPUTINFO lif;
    DWORD tickCount;
    qint32 lastInputSecondsCount;

    lif.cbSize = sizeof(LASTINPUTINFO);

    apiResult = GetLastInputInfo(&lif);

    // if last input info request failed, then assuming last input was at this moment
    if (0 == apiResult) {
        QLOG_DEBUG() << "[System\Monitor] ERROR: can`t obtain last input time info";
        return 0;
    }

    tickCount = GetTickCount();
    lastInputSecondsCount = (qint32)((tickCount - lif.dwTime) / 1000);

    return lastInputSecondsCount;
}
