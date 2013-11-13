#ifndef WINDOWSMONITOR_H
#define WINDOWSMONITOR_H

#define WINVER 0x0502

#include "ProcessInfo.h"

#include <windows.h>
#include <psapi.h>
#include <TlHelp32.h>
#include <iostream>

#include <winuser.h>
#include <oleacc.h>

#include "vendors/XMSAALib.h"
#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"

#include <QDebug>
#include <QStringList>

class ProcessActivityMonitor
{
public:
    ProcessActivityMonitor();
    ProcessInfo getForegroundProcess();
};

#endif // WINDOWSMONITOR_H
