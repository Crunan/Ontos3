#include "include/commandfilereader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

CommandFileReader::CommandFileReader(QString filePath, QString fileName)
    : commandFilePath(filePath), commandFileName(fileName)
{
}

QMap<QString, QPair<QString, QString>> CommandFileReader::readCommandsFromFile()
{
    static QRegularExpression regex("(\\d{3})\\s*=\\s*(\\w+)\\s*\\{([^}]*)\\}");

    QMap<QString, QPair<QString, QString>> commandMap;

    QFile file(commandFilePath + "/" + commandFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open the file:" << file.errorString();
        return commandMap;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch())
        {
            QString paramIndex = match.captured(1);
            QString name = match.captured(2);
            QString description = match.captured(3);

            commandMap[name] = qMakePair(paramIndex, description);
        }
        else
        {
            qDebug() << "Invalid line format:" << line;
        }
    }

    file.close();

    return commandMap;
}



QString CommandFileReader::getCommandFilePath() const
{
    return commandFilePath;
}

void CommandFileReader::setCommandFilePath(QString path)
{
    commandFilePath = path;
}

QString CommandFileReader::getCommandFileName() const
{
    return commandFileName;
}

void CommandFileReader::setCommandFileName(QString fileName)
{
    commandFileName = fileName;
}
