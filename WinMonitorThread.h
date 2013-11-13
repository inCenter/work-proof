#ifndef WINMONITORTHREAD_H
#define WINMONITORTHREAD_H

#include "windows/ProcessInfo.h"
#include "system/Monitor.h"

#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"

#include <QThread>

class QTimer;

class WinMonitorThread : public QThread
{
    Q_OBJECT

    QTimer * m_scanTimer = nullptr;

    bool m_isIdleModeOn = false;

public:
    explicit WinMonitorThread(QObject *parent = 0);
    void run();
    
signals:
    void onTimelogEvent(const ProcessInfo &);
    void onMonitoringResume();

public slots:
    void processCurrentWindow();
    void pauseFor5Minutes();
    void startMonitoring();
};

#endif // WINMONITORTHREAD_H
