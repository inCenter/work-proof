#ifndef MONITOR_H
#define MONITOR_H

#include <QString>
#include <QDebug>
#include <windows.h>
#include <lm.h>
#include <iphlpapi.h>
#include "system/Info.h"

#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"

class SystemMonitor
{
    QString _getUsername();
    QString _getWorkgroup();
    QString _getMac();
    QString _getTimezone();

public:
    SystemMonitor();

    SystemInfo * getInfo();
    qint32 getLastInputSeconds();
};

#endif // MONITOR_H
