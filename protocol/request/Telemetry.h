#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QVector>
#include <QVariantMap>
#include <QDateTime>
#include <QDebug>

#include "windows/ProcessActivityRecord.h"
#include "protocol/request/Header.h"


class Telemetry : public Header
{
    QVector <ProcessActivityRecord> m_timelogs;

public:
    Telemetry();

    QVector <ProcessActivityRecord> Timelogs()              { return m_timelogs; }
    void Timelogs(QVector <ProcessActivityRecord> timelogs) { m_timelogs = timelogs; }

    QVariantMap toMap();
};

#endif // TELEMETRY_H
