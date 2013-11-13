#ifndef SESSIONSTATE_H
#define SESSIONSTATE_H

#include <QDebug>
#include <QString>

enum SessionStates
{
    SESSION_NOT_AUTHORIZED,         // client not authorized yet
    SESSION_AUTH_REPLY_WAIT,        // authorization request sent
    SESSION_TELEMETRY_REPLY_WAIT,   // telemetry request sent
    SESSION_STATS_REPLY_WAIT,       // stats request sent
    SESSION_AUTHORIZED              // received successful authorization reply
};

class SessionState
{
    SessionStates m_state;
    QString       m_token;

public:
    SessionState() = default;
    SessionState(SessionStates state);

    SessionStates State()                                    { return m_state; }
    void          State(SessionStates current_state)         { m_state = current_state; }

    QString       Token()                                    { return m_token; }
    void          Token(QString token)                       { m_token = token; }
};

#endif // SESSIONSTATE_H
