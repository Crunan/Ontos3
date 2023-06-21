#ifndef COMMANDFILEREADER_H
#define COMMANDFILEREADER_H

#include <QString>

class CommandFileReader
/* This class handles reading the command definitions,
 * descriptions, and values from a file and
 * populating the CommandMap.
 */
{
public:
    CommandFileReader(QString filePath, QString fileName);

    QMap<QString, QPair<QString, QString>> readCommandsFromFile();

    QString getCommandFilePath() const;
    void setCommandFilePath(QString path);

    QString getCommandFileName() const;
    void setCommandFileName(QString fileName);

private:
    QString commandFilePath;
    QString commandFileName;
};


#endif // COMMANDFILEREADER_H
