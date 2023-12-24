#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <QHash>
#include <QObject>
#include "UtilitiesAndConstants.h"

QFile* Logger::logFile = Q_NULLPTR;
bool Logger::isInit = false;
QHash<QtMsgType, QString> Logger::contextNames = {
    {QtMsgType::QtDebugMsg,     " Debug  "},
    {QtMsgType::QtInfoMsg,      "  Info  "},
    {QtMsgType::QtWarningMsg,   "Warning "},
    {QtMsgType::QtCriticalMsg,  "Critical"},
    {QtMsgType::QtFatalMsg,     " Fatal  "}
};

void Logger::init()
{
    if (isInit)
    {
        return;
    }
    
    QString fileTimestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    
    //Create log file
    logFile = new QFile;
    logFile->setFileName(QString(LOG_DIRECTORY) + "Log" + fileTimestamp + ".log");
    logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

#ifndef QT_DEBUG
    //Redirect logs to messageOutput
    qInstallMessageHandler(Logger::messageOutput);
#endif

    Logger::isInit = true;
}

void Logger::clean()
{
    if (logFile != Q_NULLPTR)
    {
        logFile->close();
        delete logFile;
    }
}

void Logger::messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (type == QtDebugMsg)
        return; // no debug messages in the logfile
    QString log = QObject::tr("%1 | %2 | %3 | %4 | %5 | %6\n").
            arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")).
            arg(Logger::contextNames.value(type)).
            arg(context.line).
            arg(QString(context.file).
                section('\\', -1)).         // File name without file path
            arg(QString(context.function).
                section('(', -2, -2).       // Function name only
                section(' ', -1).
                section(':', -1)).
            arg(msg);

    logFile->write(log.toLocal8Bit());
    logFile->flush();
}

void Logger::logDebug(QString debugMsg) {
    qDebug() << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << " " << debugMsg;
}
void Logger::logInfo(QString infoMsg) {
    qInfo() << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << " "  << infoMsg;
}
void Logger::logWarning(QString warningMsg) {
    qWarning() << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << " "  << warningMsg;
}
void Logger::logCritical(QString criticalMsg) {
    qCritical() << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << " "  << criticalMsg;
}
void Logger::logFatal() {
    qFatal("Fatal Log!");
}

