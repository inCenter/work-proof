#ifndef WINDOWTIMELOG_H
#define WINDOWTIMELOG_H

#include <QString>

#include "ProcessInfo.h"

class ProcessActivityRecord
{
private:
    qint64 m_startTimestamp;
    qint64 m_endTimestamp;
    ProcessInfo m_procInfo;

public:
    ProcessActivityRecord() = default;
    ProcessActivityRecord(qint64, qint64, const ProcessInfo &);

    void    StartTimestamp(qint64 timestamp)    { m_startTimestamp = timestamp; }
    qint64  StartTimestamp()                    { return m_startTimestamp; }

    void    EndTimestamp(qint64 timestamp)      { m_endTimestamp = timestamp; }
    qint64  EndTimestamp()                      { return m_endTimestamp; }

    void Process(const ProcessInfo & pi) { m_procInfo = pi; }
    ProcessInfo Process()                { return m_procInfo; }

    bool operator==(ProcessActivityRecord& rec);
};

#endif // WINDOWTIMELOG_H
