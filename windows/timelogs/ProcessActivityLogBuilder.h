#ifndef WINDOWTIMELOGSMANAGER_H
#define WINDOWTIMELOGSMANAGER_H

#include "windows/timelogs/ProcessActivitySnapshot.h"
#include "windows/ProcessActivityRecord.h"

#include <QVector>
#include <QDebug>

#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"

class ProcessActivityLogBuilder
{
private:
    QVector <ProcessActivitySnapshot> m_snapshotsList;
    QVector <ProcessActivitySnapshot> m_alreadySentSnapshotsList;

public:
    ProcessActivityLogBuilder();
    void addSnapshot(const ProcessActivitySnapshot &);
    void clearSnapshots();
    QVector <ProcessActivityRecord> getCurrentTimelog();
    int getCurrentTimelogsCount();
};

#endif // WINDOWTIMELOGSMANAGER_H
