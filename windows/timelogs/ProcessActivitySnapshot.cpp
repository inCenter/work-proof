#include "ProcessActivitySnapshot.h"

ProcessActivitySnapshot::ProcessActivitySnapshot(
        const QDateTime & dt, const ProcessInfo & pi)
{
    Time(dt);
    Process(pi);
}


bool ProcessActivitySnapshot::operator ==(ProcessActivitySnapshot& snap)
{
    return (this->m_proc.Title()        == snap.Process().Title() &&
            this->m_proc.ProcessName()  == snap.Process().ProcessName() &&
            this->m_time                == snap.Time());
}


bool ProcessActivitySnapshot::operator !=(ProcessActivitySnapshot& snap)
{
    return (this->m_proc.Title()        != snap.Process().Title() ||
            this->m_proc.ProcessName()  != snap.Process().ProcessName() ||
            this->m_time                != snap.Time());
}
