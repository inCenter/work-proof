#include "windows/timelogs/ProcessActivityLogBuilder.h"
#include "windows/timelogs/ProcessActivitySnapshot.h"
#include "windows/ProcessActivityRecord.h"


ProcessActivityLogBuilder::ProcessActivityLogBuilder()
{
}


void ProcessActivityLogBuilder::addSnapshot(const ProcessActivitySnapshot & snapshot)
{
    m_snapshotsList.append(snapshot);
}


void ProcessActivityLogBuilder::clearSnapshots()
{
    if (m_snapshotsList.size() > 1) {
        ProcessActivitySnapshot lastSentSnapshot, currentSnapshot, currentSentSnapshot;
        QVector <ProcessActivitySnapshot> newSnapshotsList;
        bool snapshotSent = false;

        lastSentSnapshot = m_alreadySentSnapshotsList.last();


        for (int currentSnapshotIndex = 0; currentSnapshotIndex < m_snapshotsList.size(); currentSnapshotIndex++) {
            currentSnapshot = m_snapshotsList.at(currentSnapshotIndex);
            snapshotSent = false;
            for (int currentSentSnapshotIndex = 0; currentSentSnapshotIndex < m_alreadySentSnapshotsList.size(); currentSentSnapshotIndex++) {
                currentSentSnapshot = m_alreadySentSnapshotsList.at(currentSentSnapshotIndex);
                if (currentSnapshot == currentSentSnapshot && currentSentSnapshot != lastSentSnapshot) {
                    snapshotSent = true;
                }
            }

            if (!snapshotSent) {
                newSnapshotsList.append(currentSnapshot);
                QLOG_DEBUG() << "Saved snapshot # " << currentSnapshotIndex << ": " << currentSnapshot.Process().ProcessName() << ': ' << currentSnapshot.Time();
            }
        }

        m_snapshotsList = newSnapshotsList;
    } else {
        m_snapshotsList.clear();
    }
    m_alreadySentSnapshotsList.clear();
}


int ProcessActivityLogBuilder::getCurrentTimelogsCount()
{
    return m_snapshotsList.size();
}


QVector<ProcessActivityRecord> ProcessActivityLogBuilder::getCurrentTimelog()
{
    ProcessActivitySnapshot activityStartSnp;
    ProcessActivitySnapshot activityPrevSnp;

    QVector <ProcessActivityRecord> timelogsList;
    int currentSnapshotIndex = 0;
    int snapshotsCount = m_snapshotsList.size();

    for (auto & snp : m_snapshotsList)
    {
        currentSnapshotIndex++;


        // if last point is first point .. just exit and return empty list

        if (currentSnapshotIndex == snapshotsCount &&
            activityStartSnp.Process().Title().trimmed().isEmpty() &&
            activityStartSnp.Process().ProcessName().trimmed().isEmpty()) {
            continue;
        }


        // initializing start and prev snapshots

        if (1 == currentSnapshotIndex)
        {
            activityStartSnp = snp;
            activityPrevSnp  = snp;
            m_alreadySentSnapshotsList.append(snp);
            continue;
        }


        // if last point processing then creating timelog immediately

        if (currentSnapshotIndex == snapshotsCount)
        {
            m_alreadySentSnapshotsList.append(snp);
            ProcessActivityRecord activity(activityStartSnp.Time().toMSecsSinceEpoch(),
                                           snp.Time().toMSecsSinceEpoch(),
                                           activityPrevSnp.Process());

            timelogsList.append(activity);
            continue;
        }


        auto currentProc = snp.Process();

        if ( currentProc.Title()       != activityPrevSnp.Process().Title() ||
             currentProc.ProcessName() != activityPrevSnp.Process().ProcessName() )
        {
            ProcessActivityRecord activity(activityStartSnp.Time().toMSecsSinceEpoch(),
                                           snp.Time().toMSecsSinceEpoch(),
                                           activityPrevSnp.Process());

            timelogsList.append(activity);


            // redefine start snapshot and prev snapshot

            activityStartSnp = snp;
            activityPrevSnp = snp;
        }

        m_alreadySentSnapshotsList.append(snp);
    }

    return timelogsList;
}
