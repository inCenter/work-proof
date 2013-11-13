#include "windows/ProcessActivityMonitor.h"
#include "windows/ProcessInfo.h"

static QString g_currentAddressBarValue;
int g_foundKeywords = 0;
int g_iAccParamIndex = 0;

static BOOL CALLBACK MSAAEnumFirefoxAddressBarProc(IAccessible *,
                                            VARIANT *,
                                            HWND /*hwndChild*/,
                                            int /*nLevel*/,
                                            LPARAM /*lParam*/);


ProcessActivityMonitor::ProcessActivityMonitor()
{
    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
}

ProcessInfo ProcessActivityMonitor::getForegroundProcess()
{
    HWND hwnd               = NULL;
    DWORD dwPID             = 0;
    WCHAR wString[257]      = {0};
    WCHAR wProcessName[257] = {0};
    WCHAR wWindowClass[257] = {0};
    HMODULE hMod;
    DWORD cbNeeded;

    hwnd = GetForegroundWindow();
    if (hwnd)
    {
        GetWindowText(hwnd, wString, 256);
        GetWindowThreadProcessId(hwnd, &dwPID);
        GetClassName(hwnd, wWindowClass, 256);
    }

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, dwPID );

    if ( 0 != hProcess) {
        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ) {
            GetModuleBaseName( hProcess, hMod, wProcessName, sizeof(wProcessName) / sizeof(WCHAR) );
        }
    }

    QString resultTitle = QString::fromWCharArray(wString);

    if ("firefox.exe" == QString::fromWCharArray(wProcessName).toLower())
    {
        g_currentAddressBarValue = "";
        g_iAccParamIndex = 0;
        g_foundKeywords = 0;

        IAccessible *pIAccParent = NULL;
        AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, IID_IAccessible, (void**)&pIAccParent);

        CXMSAALib MSAALib;
        MSAALib.EnumAccessible((HWND)hwnd, MSAAEnumFirefoxAddressBarProc, 0);

        resultTitle = g_currentAddressBarValue;
    }
    else if ("opera.exe" == QString::fromWCharArray(wProcessName).toLower())
    {
        HWND hOpera;
        WCHAR wAddress[257] = {0};

        hOpera = FindWindowEx(hwnd, NULL, L"ViewsTextfieldEdit", NULL);
        SendMessage(hOpera, WM_GETTEXT, 257, (LPARAM)wAddress);

        resultTitle = QString::fromWCharArray(wAddress);
    }
    else if ("chrome.exe" == QString::fromWCharArray(wProcessName).toLower())
    {
        HWND hChrome;
        WCHAR wAddress[257] = {0};

        hChrome = FindWindowEx(hwnd, NULL, L"Chrome_OmniboxView", NULL);
        SendMessage(hChrome, WM_GETTEXT, 257, (LPARAM)wAddress);

        resultTitle = QString::fromWCharArray(wAddress);
    }
    else if ("iexplore.exe" == QString::fromWCharArray(wProcessName).toLower())
    {
        HWND hAddress, hWorkerW, hReBarWindow32, hAddressBandRoot;
        WCHAR wAddress[257] = {0};

        hWorkerW = FindWindowEx(hwnd, NULL, L"WorkerW", NULL);
        hReBarWindow32 = FindWindowEx(hWorkerW, NULL, L"ReBarWindow32", NULL);
        hAddressBandRoot = FindWindowEx(hReBarWindow32, NULL, L"Address Band Root", NULL);
        hAddress = FindWindowEx(hAddressBandRoot, NULL, L"Edit", NULL);

        SendMessage(hAddress, WM_GETTEXT, 257, (LPARAM)wAddress);

        resultTitle = QString::fromWCharArray(wAddress);
    }

    QLOG_DEBUG() << "Grabbed window: " << QString::fromWCharArray(wProcessName) << " - " << resultTitle;

    return ProcessInfo(resultTitle, QString::fromWCharArray(wProcessName));
}


static BOOL CALLBACK MSAAEnumFirefoxAddressBarProc(IAccessible *pAccChild,
                                            VARIANT *pvarChild,
                                            HWND /*hwndChild*/,
                                            int /*nLevel*/,
                                            LPARAM /*lParam*/)
{
    g_iAccParamIndex++;

    if (g_iAccParamIndex > 252) {
        return FALSE;
    }

    if (pAccChild && pvarChild)
    {
        // check name
        BSTR szName;
        CXMSAALib::GetObjectName(pAccChild, pvarChild, szName, 255);

        // name and role are ok - check value string for "http"
        BSTR szValue;
        CXMSAALib::GetObjectValue(pAccChild, pvarChild, szValue, 255);

        long childCount = 0;
        CXMSAALib::GetObjectChildCount(pAccChild, childCount);

        QString etalonElement = "Введите поисковый запрос или адрес";
        QString etalonElementEng = "Search or enter address";
        if (etalonElement == QString::fromWCharArray(szName) ||
            etalonElementEng == QString::fromWCharArray(szName)) {
            g_foundKeywords++;
            if (2 == g_foundKeywords) {
                g_currentAddressBarValue = QString::fromWCharArray(szValue);
            }
        }
    }
    return TRUE;
}
