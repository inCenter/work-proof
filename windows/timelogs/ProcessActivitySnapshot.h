#ifndef WINDOWTIMELOGSNAPSHOT_H
#define WINDOWTIMELOGSNAPSHOT_H

#include "windows/ProcessInfo.h"

#include <QDateTime>

class ProcessActivitySnapshot
{
    QDateTime m_time;
    ProcessInfo m_proc;

public:
    ProcessActivitySnapshot() = default;
    ProcessActivitySnapshot(const QDateTime &, const ProcessInfo &);

    void Time(QDateTime t) { m_time = t; }
    QDateTime Time()       { return m_time; }

    void Process(const ProcessInfo & p) { m_proc = p; }
    ProcessInfo Process()               { return m_proc; }

    bool operator==(ProcessActivitySnapshot& snap);
    bool operator!=(ProcessActivitySnapshot& snap);
};

#endif // WINDOWTIMELOGSNAPSHOT_H
