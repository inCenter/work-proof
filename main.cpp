#include <QtGui/QApplication>
#include <QDebug>
#include <QJson/Parser>
#include <QDir>
#include <QDesktopServices>
#include <QtSingleApplication>

#include "TMonWindow.h"
#include "windows/timelogs/ProcessActivityLogBuilder.h"

#include "vendors/QsLog.h"
#include "vendors/QsLogDest.h"


int main(int argc, char *argv[])
{
    qRegisterMetaType<ProcessInfo>("ProcessInfo");
    qRegisterMetaType<QVector <ProcessActivityRecord>>("QVector <ProcessActivityRecord>");
    qRegisterMetaType<QVector <QString>>("QVector <QString>");
    qRegisterMetaType<SessionStates>("SessionStates");


    // Init encoding settings

    QTextCodec::setCodecForTr(QTextCodec::codecForName ("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName ("utf-8"));

    // Init application

    QtSingleApplication app(argc, argv);
    if (app.isRunning())
        return 0;

    // Init logger
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "\\" + "wproof_debug.txt");
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );
    logger.addDestination(debugDestination.get());
    logger.addDestination(fileDestination.get());

    TMonWindow window;

    return app.exec();
}
