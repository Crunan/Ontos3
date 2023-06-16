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


    static void logDebug(QString debugMsg) const;
    static void logInfo(QString infoMsg) const;
    static void logWarning(QString warningMsg) const;
    static void logCritical(QString criticalMsg) const;
    static void logFatal() const;
};

#endif // LOGGER_H
