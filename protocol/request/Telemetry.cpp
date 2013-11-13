#include "Telemetry.h"

Telemetry::Telemetry()
{
}

QVariantMap Telemetry::toMap()
{
    QVariantMap request;
    QVariantMap header;
    QVariantMap windowsTimelogsHeader;

    QVariantMap windowTimelog;
    QVariantMap timelogs;

    int timelogIndex = 0;

    // header

    header.insert("sessionToken", SessionToken());
    header.insert("mac",          Mac());
    header.insert("timestamp",    Timestamp());
    header.insert("timezone",     Timezone());

    // windows timelogs
    for (auto & timelog : Timelogs())
    {
        windowTimelog.clear();

        auto procInfo = timelog.Process();

        windowTimelog.insert("procName",    procInfo.ProcessName());
        windowTimelog.insert("windowName",  procInfo.Title());
        windowTimelog.insert("startTimestamp", timelog.StartTimestamp());
        windowTimelog.insert("endTimestamp", timelog.EndTimestamp());

        timelogs.insertMulti(QString::number(timelogIndex), windowTimelog);

        timelogIndex++;
    }

    windowsTimelogsHeader.insert("windowsTimelogs", timelogs);

    request.insert("header", header);
    request.insert("telemetry", windowsTimelogsHeader);

    return request;
}
