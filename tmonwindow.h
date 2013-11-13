#ifndef TMONWINDOW_H
#define TMONWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTextCodec>
#include <QTimer>
#include <QStringList>
#include <QSettings>
#include <QVector>
#include <QDesktopServices>

#include "ProtocolThread.h"
#include "windows/timelogs/ProcessActivityLogBuilder.h"
#include "windows/ProcessActivityRecord.h"
#include "system/Monitor.h"
#include "system/Info.h"


namespace Ui {
class TMonWindow;
}

class TMonWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TMonWindow(QWidget *parent = 0);
    ~TMonWindow();
    void setTimelogManager(ProcessActivityLogBuilder *);

public slots:
    void closeWindow();
    void openDashboard();
    void timelogReceived(const ProcessInfo &);
    void processRequestForTimelog();
    void setIconPaused();
    void setIconUnpaused();
    void showTrayStats(QSystemTrayIcon::ActivationReason);
    void updateStats(QVector <QString>);
    void updateSessionState(SessionStates);
    void showTrayMessage(QVector <QString>);
    
signals:
    void reponseForTimelogsRequest(QVector <ProcessActivityRecord>);
    void exitApp();

private:
    Ui::TMonWindow *ui;
    ProcessActivityLogBuilder * m_logBuilder;
    QSystemTrayIcon * m_trayIcon;

    QString m_statProductivityPercent;
    QString m_statProductiveSeconds;
};

#endif // TMONWINDOW_H
