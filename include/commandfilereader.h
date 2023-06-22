#ifndef COMMANDFILEREADER_H
#define COMMANDFILEREADER_H

#include <QString>
#include <QMap>
#include <QPair>

class CommandFileReader
/* This class handles reading the command definitions,
 * descriptions, and values from a file and
 * populating the CommandMap.
 */
{
public:
    CommandFileReader(  );

    const QMap<QString, QPair<QString, QString>>& getCommandMap() const;
    QMap<QString, QPair<QString, QString>> readCommandsFromFile();

    QString getCommandFilePath() const;
    void setCommandFilePath(QString path);

    QString getCommandFileName() const;
    void setCommandFileName(QString fileName);

private:
    QString commandFilePath_;
    QString commandFileName_;
    QMap<QString, QPair<QString, QString>> commandMap_;
};


#endif // COMMANDFILEREADER_H
