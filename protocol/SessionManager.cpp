#include "SessionManager.h"

bool SessionManager::requestState(SessionStates requested_state)
{
    SessionStates currentState = m_state->State();

    switch (requested_state)
    {
        // network problems probably
        case SESSION_NOT_AUTHORIZED:
            if (SESSION_NOT_AUTHORIZED  != currentState &&
                SESSION_AUTHORIZED      != currentState) {
                return true;
            }
            return false;


        // trying to authorize
        case SESSION_AUTH_REPLY_WAIT:
            if (SESSION_NOT_AUTHORIZED == currentState) {
                return true;
            }
            return false;


        // have telemetry data to send
        case SESSION_TELEMETRY_REPLY_WAIT:
            if (SESSION_AUTHORIZED == currentState) {
                return true;
            }
            return false;


        // updating local stats
        case SESSION_STATS_REPLY_WAIT:
            if (SESSION_AUTHORIZED == currentState) {
                return true;
            }
            return false;


        case SESSION_AUTHORIZED:
            if (SESSION_AUTH_REPLY_WAIT == currentState ||
                SESSION_TELEMETRY_REPLY_WAIT == currentState ||
                SESSION_STATS_REPLY_WAIT == currentState) {
                return true;
            }
            return false;

        default:
            return false;
   }

}


SessionStates SessionManager::State()
{
    return m_state->State();
}


void SessionManager::State(SessionStates destination_state)
{
    m_state->State(destination_state);
    emit sigSessionStatusUpdated(destination_state);
}


void SessionManager::Token(QString token)
{
    m_state->Token(token);
}


QString SessionManager::Token()
{
    return m_state->Token();
}
