#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <QDebug>
#include <QFile>
#include <QHash>

class Logger
{
private:
    static QFile* logFile;

    static bool isInit;

    static QHash<QtMsgType, QString> contextNames;

public:
    static void init();

    static void clean();

    static void logAxesStatus();

    static void messageOutput(QtMsgType type, const QMessageLogContext& context,
                              const QString& msg);


    static void logDebug(QString debugMsg);
    static void logInfo(QString infoMsg);
    static void logWarning(QString warningMsg);
    static void logCritical(QString criticalMsg);
    static void logFatal();
};

#endif // LOGGER_H
