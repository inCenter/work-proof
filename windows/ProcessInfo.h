#ifndef WINDOW_H
#define WINDOW_H

#include <QString>

class ProcessInfo
{
    QString m_title = "";
    QString m_procName = "";

public:
    ProcessInfo() = default;
    ProcessInfo(const QString & title, const QString & process_name);

    void    Title(const QString & t) { m_title = t; }
    QString Title()                  { return m_title; }

    void    ProcessName(const QString & pn) { m_procName = pn; }
    QString ProcessName()                   { return m_procName; }
};

#endif // WINDOW_H
