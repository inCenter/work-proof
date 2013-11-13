#include "WinMonitorThread.h"
#include "windows/ProcessActivityMonitor.h"

#include <QTimer>
#include <QDebug>


WinMonitorThread::WinMonitorThread(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
}

void WinMonitorThread::run()
{
    m_scanTimer = new QTimer(this);
    connect(m_scanTimer, SIGNAL(timeout()), SLOT(processCurrentWindow()));
    m_scanTimer->start(2000);

    exec();
}

void WinMonitorThread::processCurrentWindow()
{
    SystemMonitor sysMon;
    ProcessActivityMonitor paMon; // FIXME make it a member

    // pause snapshoting after 10 minute idle
    if (sysMon.getLastInputSeconds() <= 60*10) {
        if (m_isIdleModeOn) {
            QLOG_DEBUG() << "[WinMonitor]: Awaken from sleeping";
            m_isIdleModeOn = false;
        }
        emit onTimelogEvent(paMon.getForegroundProcess()); // ...or a static function
    } else {
        if (!m_isIdleModeOn) {
            QLOG_DEBUG() << "[WinMonitor]: SystemIdle detected. Sleeping..";
            m_isIdleModeOn = true;
        }
    }
}


void WinMonitorThread::pauseFor5Minutes()
{
    m_scanTimer->stop();
    QTimer::singleShot(300000, this, SLOT(startMonitoring()));
}


void WinMonitorThread::startMonitoring()
{
    m_scanTimer->start(2000);
    emit onMonitoringResume();
}
