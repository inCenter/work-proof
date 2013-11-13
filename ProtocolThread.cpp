#include "ProtocolThread.h"

ProtocolThread::ProtocolThread(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
}


void ProtocolThread::run()
{
    QLOG_DEBUG() << "Starting protocol thread";

    m_sessionManager = new SessionManager(new SessionState(SESSION_NOT_AUTHORIZED));
    connect(m_sessionManager, SIGNAL(sigSessionStatusUpdated(SessionStates)),
                        this, SLOT(sessionStatusUpdated(SessionStates)) );

    m_authTimer = new QTimer(this);
    connect(m_authTimer, SIGNAL(timeout()), SLOT(sendAuthRequest()));
    m_authTimer->start(ProtocolThread::AUTH_SENT_PERIOD);

    m_telemetryTimer = new QTimer(this);
    connect(m_telemetryTimer, SIGNAL(timeout()), SLOT(sendTelemetryRequest()));

    m_statsTimer = new QTimer(this);
    connect(m_statsTimer, SIGNAL(timeout()), SLOT(sendStatsRequest()));

    exec();
}


void ProtocolThread::sendAuthRequest()
{
    if (!m_sessionManager->requestState(SESSION_AUTH_REPLY_WAIT)) {
        QLOG_DEBUG() << "Can`t send auth request. Waiting next attempt..";
        return;
    }

    // Building request

    Auth authRequest;
    authRequest.LicenseCode( m_config->value("api/license").toString() );
    authRequest.Workgroup( m_systemInfo->Workgroup() );
    authRequest.Username( m_systemInfo->UserName() );
    authRequest.Mac( m_systemInfo->MacAddress() );
    authRequest.Timezone( m_systemInfo->Timezone() );

    QNetworkRequest request;
    request.setUrl(QUrl( m_config->value("api/host").toString() ));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QJson::Serializer serializer;
    QByteArray requestParams = "data=" + serializer.serialize(authRequest.toMap()).toBase64();

    QLOG_DEBUG() << "Auth sent";

    // Wating for response

    m_authConnManager = new QNetworkAccessManager(this);
    connect(m_authConnManager, SIGNAL(finished(QNetworkReply*)), SLOT(authReplyReceived(QNetworkReply*)));
    connect(m_authConnManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
    m_authConnManager->post(request, requestParams);
    m_sessionManager->State(SESSION_AUTH_REPLY_WAIT);
}


void ProtocolThread::authReplyReceived(QNetworkReply* reply)
{
    QByteArray rawResponse;

    while(true) // simulating exceptions
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            QLOG_DEBUG() << "Auth reply error code: " << reply->error();
            m_sessionManager->State(SESSION_NOT_AUTHORIZED);
            break;
        }

        rawResponse = reply->readAll();

        QLOG_DEBUG() << "Auth response: " << rawResponse;

        QJson::Parser parser;
        bool parseResult;

        QVariantMap jsonResponse = parser.parse(rawResponse, &parseResult).toMap();
        if (!parseResult) {
            m_sessionManager->State(SESSION_NOT_AUTHORIZED);
            break;
        }

        QVariantMap responseBody = jsonResponse["response"].toMap();

        if ( responseBody["status"].toString() == "ERROR") {
            QString errorCode = responseBody["errorCode"].toString();
            QString errorText = responseBody["errorMessage"].toString();
            QVector <QString> errorMessage;
            errorMessage.append("Connection error");
            if (errorCode == "501" || errorCode == "502") {
                errorMessage.append(errorText);
            } else {
                break;
            }
            emit sigConnectionError(errorMessage);
            m_sessionManager->State(SESSION_NOT_AUTHORIZED);
            break;
        }


        if (!responseBody.contains("sessionToken")) {
            m_sessionManager->State(SESSION_NOT_AUTHORIZED);
            break;
        }

        if ( responseBody["sessionToken"].toString().length() <= 0 ) {
            m_sessionManager->State(SESSION_NOT_AUTHORIZED);
            break;
        }

        m_sessionManager->Token(responseBody["sessionToken"].toString());
        m_sessionManager->State(SESSION_AUTHORIZED);

        m_authTimer->stop();
        m_telemetryTimer->start(ProtocolThread::TELEMERY_SENT_PERIOD);
        m_statsTimer->start(ProtocolThread::STAT_SENT_PERIOD);

        break;
    }

    m_authConnManager->deleteLater();
    reply->deleteLater();
    reply->close();
}


void ProtocolThread::sendTelemetryRequest()
{
    // skipping if there are no new timelogs
    if (m_timelogBuilder->getCurrentTimelogsCount() <= 1) {
        return;
    }

    if (!m_sessionManager->requestState(SESSION_TELEMETRY_REPLY_WAIT)) {
        QLOG_DEBUG() << "Can`t send telemetry request. Waiting next attempt..";
        return;
    }

    Telemetry telemetryRequest;

    telemetryRequest.SessionToken( m_sessionManager->Token() );
    telemetryRequest.Mac( m_systemInfo->MacAddress() );
    telemetryRequest.Timelogs( m_timelogBuilder->getCurrentTimelog() );
    telemetryRequest.Timestamp( QDate::currentDate().toString() );
    telemetryRequest.Timezone( m_systemInfo->Timezone() );

    QNetworkRequest request;
    request.setUrl(QUrl( m_config->value("api/host").toString() ));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QJson::Serializer serializer;
    QByteArray requestParams = "data=" + serializer.serialize(telemetryRequest.toMap()).toBase64();

    QLOG_DEBUG() << "Telemetry sent";

    // Wating for response

    m_teleConnManager = new QNetworkAccessManager(this);
    connect(m_teleConnManager, SIGNAL(finished(QNetworkReply*)), SLOT(telemetryReplyReceived(QNetworkReply*)));
    connect(m_teleConnManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
    m_teleConnManager->post(request, requestParams);
    m_sessionManager->State(SESSION_TELEMETRY_REPLY_WAIT);
}


void ProtocolThread::telemetryReplyReceived(QNetworkReply* reply)
{
    while(true) // simulating exceptions
    {
        if (reply->error() != QNetworkReply::NoError) {
            QLOG_DEBUG() << "Telemetry reply error code: " << reply->error();
            break;
        }

        QLOG_DEBUG() << "Telemetry response: " << reply->readAll();

        m_sessionManager->State(SESSION_AUTHORIZED);
        m_timelogBuilder->clearSnapshots();

        m_teleConnManager->deleteLater();
        reply->deleteLater();
        reply->close();

        return;
    }

    // connnection lost. setup auth timer
    m_sessionManager->State(SESSION_NOT_AUTHORIZED);

    m_telemetryTimer->stop();
    m_statsTimer->stop();
    m_authTimer->start(ProtocolThread::AUTH_SENT_PERIOD);

    m_teleConnManager->deleteLater();
    reply->deleteLater();
    reply->close();
}


void ProtocolThread::sendStatsRequest()
{
    if (!m_sessionManager->requestState(SESSION_STATS_REPLY_WAIT)) {
        QLOG_DEBUG() << "Can`t send stats request. Waiting next attempt..";
        return;
    }

    Configuration configurationRequest;

    configurationRequest.SessionToken( m_sessionManager->Token() );
    configurationRequest.Mac( m_systemInfo->MacAddress() );
    configurationRequest.Timestamp( QDate::currentDate().toString() );
    configurationRequest.Timezone( m_systemInfo->Timezone() );

    QNetworkRequest request;
    request.setUrl(QUrl( m_config->value("api/host").toString() ));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QJson::Serializer serializer;
    QByteArray requestParams = "data=" + serializer.serialize(configurationRequest.toMap()).toBase64();

    QLOG_DEBUG() << "Stat sent";

    m_statConnManager = new QNetworkAccessManager(this);
    connect(m_statConnManager, SIGNAL(finished(QNetworkReply*)), SLOT(statsReplyReceived(QNetworkReply*)));
    connect(m_statConnManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
    m_statConnManager->post(request, requestParams);
    m_sessionManager->State(SESSION_STATS_REPLY_WAIT);
}


void ProtocolThread::statsReplyReceived(QNetworkReply* reply)
{
    QByteArray rawResponse;

    while(true) // simulating exceptions
    {
        if (reply->error() != QNetworkReply::NoError) {
            QLOG_DEBUG() << "Stats reply error code: " << reply->error();
            break;
        }

        rawResponse = reply->readAll();

        QLOG_DEBUG() << "Stat response: " << rawResponse;

        QJson::Parser parser;
        bool parseResult;

        QVariantMap jsonResponse = parser.parse(rawResponse, &parseResult).toMap();
        if (!parseResult) {
            break;
        }

        QVariantMap responseBody = jsonResponse["response"].toMap();

        if (!responseBody.contains("productivityPercent")) {
            break;
        }

        if ( responseBody["productivityPercent"].toString().length() <= 0 ) {
            break;
        }

        QVector <QString> stats;
        stats.append(responseBody["productivityPercent"].toString());
        stats.append(responseBody["productiveSeconds"].toString());

        m_sessionManager->State(SESSION_AUTHORIZED);

        emit statsReceived(stats);

        m_statConnManager->deleteLater();
        reply->deleteLater();
        reply->close();

        return;
    }

    // connnection lost. setup auth timer
    m_sessionManager->State(SESSION_NOT_AUTHORIZED);

    m_telemetryTimer->stop();
    m_statsTimer->stop();
    m_authTimer->start(ProtocolThread::AUTH_SENT_PERIOD);

    m_statConnManager->deleteLater();
    reply->deleteLater();
    reply->close();
}


void ProtocolThread::sessionStatusUpdated(SessionStates new_state)
{
    emit sigSessionStatusUpdated(new_state);
}


void ProtocolThread::onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error)
{
    reply->ignoreSslErrors(error);
}
