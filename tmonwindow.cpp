#include "TMonWindow.h"
#include "WinMonitorThread.h"
#include "ui/ProductivityIndicator.h"
#include "windows/timelogs/ProcessActivityLogBuilder.h"
#include "windows/timelogs/ProcessActivitySnapshot.h"
#include "ui_tmonwindow.h"

#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QThreadPool>

TMonWindow::TMonWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TMonWindow)
{ 
    ui->setupUi(this);


    // Init settings

    auto config = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator() + "config.ini", QSettings::IniFormat);


    auto sysMonitor = new SystemMonitor();
    auto sysInfo = sysMonitor->getInfo();

    // Init timelog builder

    setTimelogManager(new ProcessActivityLogBuilder());


    // Init tray menu

    QMenu   *trayMenu               = new QMenu(this);
    QAction *dashboardAction        = new QAction( tr("Dashboard"), this);
    QAction *pause5MinutesAction    = new QAction( tr("Pause for 5 minutes"), this);
    QAction *closeAction            = new QAction( tr("Close"), this);

    trayMenu->addAction(dashboardAction);
    trayMenu->addAction(pause5MinutesAction);
    trayMenu->addAction(closeAction);


    m_trayIcon = new QSystemTrayIcon(this); // TODO load a pictogram
    m_trayIcon->setIcon(QIcon(":/icon_disconnected.png"));
    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->show();

    // Init threads

    auto winMonThread = new WinMonitorThread();
    winMonThread->start();

    auto protocolThread = new ProtocolThread();
    protocolThread->Config(config);
    protocolThread->SysInfo(sysInfo);
    protocolThread->TimelogBuilder(m_logBuilder);
    protocolThread->start();

    connect(m_trayIcon,          SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showTrayStats(QSystemTrayIcon::ActivationReason)));
    connect(pause5MinutesAction, SIGNAL(triggered()), winMonThread, SLOT(pauseFor5Minutes()));
    connect(pause5MinutesAction, SIGNAL(triggered()), this,         SLOT(setIconPaused()));
    connect(     closeAction,    SIGNAL(triggered()), SLOT(closeWindow()));
    connect(     dashboardAction,SIGNAL(triggered()), SLOT(openDashboard()));

    // new snapshot received

    connect(winMonThread, SIGNAL(onTimelogEvent(const ProcessInfo &)), SLOT(timelogReceived(const ProcessInfo &)));
    connect(winMonThread, SIGNAL(onMonitoringResume()),                SLOT(setIconUnpaused()));

    connect(protocolThread, SIGNAL(statsReceived(QVector <QString>)),       SLOT(updateStats(QVector <QString>)) );
    connect(protocolThread, SIGNAL(sigSessionStatusUpdated(SessionStates)), SLOT(updateSessionState(SessionStates)));
    connect(protocolThread, SIGNAL(sigConnectionError(QVector <QString>)),  SLOT(showTrayMessage(QVector <QString>)));
    // exitting app

    connect(this, SIGNAL(exitApp()), winMonThread, SLOT(quit()));
}


void TMonWindow::timelogReceived(const ProcessInfo & pi)
{
    ProcessActivitySnapshot snapshot(QDateTime::currentDateTime(), pi);

    m_logBuilder->addSnapshot(snapshot);
}


void TMonWindow::setTimelogManager(ProcessActivityLogBuilder * timelogs_manager)
{
    m_logBuilder = timelogs_manager;
}


void TMonWindow::openDashboard()
{
    QDesktopServices::openUrl(QUrl("http://www.work-proof.com/users/auth"));
}


void TMonWindow::closeWindow()
{
    // Sending quit signals to all threads
    emit exitApp();

    // Waiting until threads are done
    QThreadPool::globalInstance()->waitForDone();

    // exitting
    close();
}


void TMonWindow::setIconPaused()
{
    m_trayIcon->setIcon(QIcon(":/icon_paused.png"));
}


void TMonWindow::setIconUnpaused()
{
    m_trayIcon->setIcon(QIcon(":/icon.png"));
}


void TMonWindow::processRequestForTimelog()
{
    QVector <ProcessActivityRecord> timelog = m_logBuilder->getCurrentTimelog();

    emit reponseForTimelogsRequest(timelog);
}


void TMonWindow::showTrayMessage(QVector <QString> message)
{
    m_trayIcon->showMessage(message[0], message[1]);
}


void TMonWindow::showTrayStats(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::DoubleClick == reason)
    {
        ProductivityIndicator prodIndicator(m_statProductiveSeconds.toInt(), m_statProductivityPercent.toInt());

        if (!prodIndicator.getLabel().isEmpty()) {
            m_trayIcon->showMessage("Statistics", prodIndicator.getLabel());
        }
    }
}


void TMonWindow::updateStats(QVector<QString> last_stats)
{
    m_statProductivityPercent   = last_stats[0];
    m_statProductiveSeconds     = last_stats[1];
}


void TMonWindow::updateSessionState(SessionStates new_state)
{
    if (new_state == SESSION_AUTHORIZED ||
        new_state == SESSION_NOT_AUTHORIZED)
    {
        if (new_state == SESSION_AUTHORIZED) {
            m_trayIcon->setIcon(QIcon(":/icon.png"));
        } else {
            m_trayIcon->setIcon(QIcon(":/icon_disconnected.png"));
        }
    }
}


TMonWindow::~TMonWindow()
{
    delete ui;
}
