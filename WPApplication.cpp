#include "WPApplication.h"


bool WPApplication::_logError(QString error_string) {

    const QString errorsLogPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "\\" + "wproof_errors.txt");

    QFile file(errorsLogPath);
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << error_string << "\n";
    file.close();

    return true;
}


bool WPApplication::notify(QObject *receiver_, QEvent *event_)
{
    try {
        return QApplication::notify(receiver_, event_);
    } catch (std::exception &ex) {

        QString errorText;
        errorText = ex.what() + QString(":") + typeid(*event_).name() + QString(":") + qPrintable(receiver_->objectName()) + QString(":") + typeid(*receiver_).name();
        _logError(errorText);

    } catch (...) {

        QString errorText;
        errorText = typeid(*event_).name() + QString(":") + qPrintable(receiver_->objectName()) + QString(":") + typeid(*receiver_).name();
        _logError(errorText);

    }

    return false;
}
