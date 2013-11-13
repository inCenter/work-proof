#include "windows/ProcessActivityRecord.h"

ProcessActivityRecord::ProcessActivityRecord(qint64 start_timestamp, qint64 end_timestamp, const ProcessInfo & pi)
{
    StartTimestamp(start_timestamp);
    EndTimestamp(end_timestamp);
    Process(pi);
}


bool ProcessActivityRecord::operator ==(ProcessActivityRecord& rec)
{
    return (this->m_procInfo.Title()        == rec.Process().Title() &&
            this->m_procInfo.ProcessName()  == rec.Process().ProcessName() &&
            this->m_startTimestamp          == rec.StartTimestamp() &&
            this->m_endTimestamp            == rec.EndTimestamp());
}
