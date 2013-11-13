#include "ProductivityIndicator.h"

QString ProductivityIndicator::getLabel()
{
    QString statLabel = "";

    int todaySeconds = m_productivitySeconds;

    if (todaySeconds <= 0) {
        return statLabel;
    }

    int hours = (int) (todaySeconds / 3600);
    if (hours > 0) {
        statLabel += QString::number(hours) + " h ";
        todaySeconds -= hours*3600;
    }

    int minutes = (int) (todaySeconds / 60);
    if (minutes > 0) {
        statLabel += QString::number(minutes) + " min";
        todaySeconds -= minutes*60;
    }

    if (statLabel.isEmpty()) {
        statLabel = QString::number(todaySeconds) + " sec";
    }

    statLabel = QString::number(m_productivePercent) + "% / " + statLabel;

    return statLabel;
}
