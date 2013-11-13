#ifndef INFO_H
#define INFO_H

#include <QString>

class SystemInfo
{
    QString m_userName;
    QString m_workgroup;
    QString m_macAddress;
    QString m_timezone;

public:
    SystemInfo() = default;
    SystemInfo(const QString &, const QString &, const QString &, const QString &);

    void UserName(QString user_name)    { m_userName    = user_name; }
    QString UserName()                  { return m_userName; }

    void Workgroup(QString workgroup)   { m_workgroup   = workgroup; }
    QString Workgroup()                 { return m_workgroup; }

    void MacAddress(QString mac)        { m_macAddress  = mac; }
    QString MacAddress()                { return m_macAddress; }

    void Timezone(QString timezone)     { m_timezone    = timezone; }
    QString Timezone()                  { return m_timezone; }
};

#endif // INFO_H
