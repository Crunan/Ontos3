#include "include/logger.h"

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
    logFile->setFileName("ontos3.log");
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

void Logger::logDebug(QString debugMsg) {
    qDebug() << debugMsg;
}
void Logger::logInfo(QString infoMsg) {
    qInfo() << infoMsg;
}
void Logger::logWarning(QString warningMsg) {
    qWarning() << warningMsg;
}
void Logger::logCritical(QString criticalMsg) {
    qCritical() << criticalMsg;
}
void Logger::logFatal() {
    qFatal("Fatal Log!");
}

//void Logger::logAxesStatus() {
//    QString LogStr{};
//    LogStr = "LED: " + axisStatus[0] + axisStatus[1];
//    LogStr += " XSt: " + axisStatus[2];
//    LogStr += " XPos: " + axisStatus[3];
//    LogStr += " YSt: " + axisStatus[5];
//    LogStr += " YPos: " + axisStatus[6];
//    LogStr += " ZSt: " + axisStatus[8];
//    LogStr += " ZPos: " + axisStatus[9];
//    logInfo(LogStr);
//}
//void MainWindow::logStageScanInfo() {
//    if (ScanSM.getCurrentCycle() == 1) {
//        logInfo("-------------Scan Start-Up--------------"); }
//    else
//    {
//        logInfo("-------------Scan Recycle Start-Up--------------");
//        logInfo("This Cycle: " + ScanSM.getCurrentCycleString()); }

//    logInfo("Display MIN:(" + ScanSM.getminXString() + " , " + ScanSM.getminYString() + ") MAX:(" + ScanSM.getmaxXString() + " , " + ScanSM.getmaxYString()  + ")");
//    logInfo("Num Rows: " + ScanSM.getNumRowsString() + " Row Sweep: " + ScanSM.getScanSweepString());
//    logInfo("FirstX: " + ScanSM.getStartingXPositionString() + " StartY: " + ScanSM.getStartingYPositionString() + " EndY: " + ScanSM.getEndingYPositionString());
//    logInfo("Scan Speed: " + ScanSM.getScanSpeedString() + " Cycles: " + ScanSM.getScanCyclesString());
//}
