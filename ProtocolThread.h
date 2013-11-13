#ifndef PROTOCOLTHREAD_H
#define PROTOCOLTHREAD_H

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QSettings>
#include <QVector>

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslError>

#include <QJson/Parser>
#include <QJson/Serializer>

#include "windows/timelogs/ProcessActivityLogBuilder.h"

#include "protocol/SessionState.h"
#include "protocol/SessionManager.h"

#include "protocol/request/Auth.h"
#include "protocol/request/Telemetry.h"
#include "protocol/request/Configuration.h"

#include "system/Info.h"

#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"

class ProtocolThread : public QThread
{
    Q_OBJECT

    QTimer *                    m_authTimer         = nullptr;
    QTimer *                    m_telemetryTimer    = nullptr;
    QTimer *                    m_statsTimer        = nullptr;

    QSettings *                 m_config            = nullptr;
    SessionManager *            m_sessionManager    = nullptr;
    SystemInfo *                m_systemInfo        = nullptr;
    ProcessActivityLogBuilder * m_timelogBuilder    = nullptr;

    QNetworkAccessManager *     m_authConnManager   = nullptr;
    QNetworkAccessManager *     m_teleConnManager   = nullptr;
    QNetworkAccessManager *     m_statConnManager   = nullptr;

public:

    /* HINT: Period cant be synced */
    static const int AUTH_SENT_PERIOD       = 5000;
    static const int TELEMERY_SENT_PERIOD   = 61009;
    static const int STAT_SENT_PERIOD       = 50000;


    explicit ProtocolThread(QObject *parent = 0);
    void run();

    void Config(QSettings * config)                              { m_config = config; }
    void SysInfo(SystemInfo * sys_info)                          { m_systemInfo = sys_info; }
    void TimelogBuilder(ProcessActivityLogBuilder * log_builder) { m_timelogBuilder = log_builder; }

signals:
    void statsReceived(QVector <QString>);
    void sigSessionStatusUpdated(SessionStates);
    void sigConnectionError(QVector <QString>);

public slots:
    void sendAuthRequest();
    void authReplyReceived(QNetworkReply*);

    void sendTelemetryRequest();
    void telemetryReplyReceived(QNetworkReply*);

    void sendStatsRequest();
    void statsReplyReceived(QNetworkReply*);

    void sessionStatusUpdated(SessionStates);

    void onIgnoreSSLErrors(QNetworkReply* reply,QList<QSslError> error);
};

#endif // PROTOCOLTHREAD_H
