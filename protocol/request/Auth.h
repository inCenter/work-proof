#ifndef AUTH_H
#define AUTH_H

#include <QString>
#include <QVariantMap>

class Auth
{
    QString m_licenseCode;
    QString m_workgroup;
    QString m_userName;
    QString m_mac;
    QString m_timezoneUTCOffset;

public:
    Auth();

    QString LicenseCode()                               { return m_licenseCode; }
    void    LicenseCode(const QString & license_code)   { m_licenseCode = license_code; }

    QString Workgroup()                                 { return m_workgroup; }
    void    Workgroup(const QString & workgroup)        { m_workgroup = workgroup; }

    QString Username()                                  { return m_userName; }
    void    Username(const QString & user_name)         { m_userName = user_name; }

    QString Mac()                                       { return m_mac; }
    void    Mac(const QString & mac)                    { m_mac = mac; }

    QString Timezone()                                  { return m_timezoneUTCOffset; }
    void    Timezone(const QString & timezone)          { m_timezoneUTCOffset = timezone; }

    QVariantMap toMap();
};

#endif // AUTH_H
