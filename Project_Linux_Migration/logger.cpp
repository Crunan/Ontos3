#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <QHash>
#include <QObject>

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

    //Create log file
    logFile = new QFile;
    logFile->setFileName("/home/oem/Ontos3/Project_Linux_Migration/logs/ontos3.log");
    logFile->open(QIODevice::Append | QIODevice::Text);

    //Redirect logs to messageOutput
    qInstallMessageHandler(Logger::messageOutput);

    //Clear file contents
    logFile->resize(0);

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

void Logger::logDebug(QString debugMsg) const {
    qDebug() << debugMsg;
}
void Logger::logInfo(QString infoMsg) const {
    qInfo() << infoMsg;
}
void Logger::logWarning(QString warningMsg) const {
    qWarning() << warningMsg;
}
void Logger::logCritical(QString criticalMsg) const {
    qCritical() << criticalMsg;
}
void Logger::logFatal() const {
    qFatal("Fatal Log!");
}

void Logger::logAxesStatus() {
    QString LogStr{};
    LogStr = "LED: " + axisStatus[0] + axisStatus[1];
    LogStr += " XSt: " + axisStatus[2];
    LogStr += " XPos: " + axisStatus[3];
    LogStr += " YSt: " + axisStatus[5];
    LogStr += " YPos: " + axisStatus[6];
    LogStr += " ZSt: " + axisStatus[8];
    LogStr += " ZPos: " + axisStatus[9];
    logInfo(LogStr);
}
