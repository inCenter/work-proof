#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include "protocol/SessionState.h"

class SessionManager : public QObject
{
    Q_OBJECT

    SessionState * m_state;

public:
    SessionManager() = default;
    SessionManager(SessionState * state)  { m_state = state; }

    bool            requestState(SessionStates state);

    void            State(SessionStates state);
    SessionStates   State();

    QString         Token();
    void            Token(QString token);

signals:
    void sigSessionStatusUpdated(SessionStates);
};

#endif // SESSIONMANAGER_H
