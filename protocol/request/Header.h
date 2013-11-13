#ifndef HEADER_H
#define HEADER_H

#include <QString>

class Header
{
    QString m_sessionToken;
    QString m_mac;
    QString m_timestamp;
    QString m_timezone;

public:
    Header();

    void SessionToken(QString session_token)    { m_sessionToken = session_token; }
    QString SessionToken()                      { return m_sessionToken; }

    void Mac(QString mac)                       { m_mac = mac; }
    QString Mac()                               { return m_mac; }

    void Timestamp(QString timestamp)           { m_timestamp = timestamp; }
    QString Timestamp()                         { return m_timestamp; }

    void Timezone(QString timezone)             { m_timezone = timezone; }
    QString Timezone()                          { return m_timezone; }
};

#endif // HEADER_H
