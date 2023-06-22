#include "include/commandfilereader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QCoreApplication>

CommandFileReader::CommandFileReader()
    : commandFilePath_(""), commandFileName_("")
{
}

const QMap<QString, QPair<QString, QString>>& CommandFileReader::getCommandMap() const
{
    return commandMap_;
}

QMap<QString, QPair<QString, QString>> CommandFileReader::readCommandsFromFile()
{
    /*
     *     ([^=]+) captures any characters that are not an equals sign (=) before the equals sign. This corresponds to the command.
     *     (\\w+) captures one or more word characters after the equals sign. This corresponds to the name.
     *     {([^}]*)\\} captures everything between the curly braces ({}), which corresponds to the description.
    */
    static QRegularExpression regex("([^=]+)\\s*=\\s*(\\w+)\\s*\\{([^}]*)\\}");

    QMap<QString, QPair<QString, QString>> commandMap;

    QString absoluteFilePath = QCoreApplication::applicationDirPath() + "/" + commandFilePath_ + commandFileName_;

    QFile file(absoluteFilePath);
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
            QString command = match.captured(1);
            QString name = match.captured(2);
            QString description = match.captured(3).trimmed(); // Trim the captured description

            if (!description.isEmpty())
            {
                commandMap[name] = qMakePair(command, description);
            }
            else
            {
                qDebug() << "Empty description for line:" << line;
            }
        }
        else
        {
            qDebug() << "Invalid line format:" << line;
        }
    }

    file.close();

    commandMap_ = commandMap;

    return commandMap;
}




QString CommandFileReader::getCommandFilePath() const
{
    return commandFilePath_;
}

void CommandFileReader::setCommandFilePath(QString path)
{
    commandFilePath_ = path;
}

QString CommandFileReader::getCommandFileName() const
{
    return commandFileName_;
}

void CommandFileReader::setCommandFileName(QString fileName)
{
    commandFileName_ = fileName;
}
