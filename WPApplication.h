#ifndef WPAPPLICATION_H
#define WPAPPLICATION_H

#include <QFile>
#include <QTextStream>

#include <QtSingleApplication>
#include <QDesktopServices>


class WPApplication : public QtSingleApplication
{
    bool _logError(QString);

public:
    WPApplication();
    bool notify(QObject *receiver_, QEvent *event_);
};

#endif // WPAPPLICATION_H
