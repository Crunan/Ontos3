#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

#include "filedescriptor.h"
#include "filewriter.h"
#include "filereader.h"

class FileManager
{
public:
    FileManager(FileWriter& writer, FileDescriptor& metaData, FileReader& fileReader);

    void write(const QString& data);
    QString read(const QString& fileToRead) const;
    const QString getFilePath() const;
    const QString getFileName() const;

private:
    FileWriter& fileWriter;
    FileDescriptor& fileMetaData;
    FileReader& fileReader;
};

#endif // FILEMANAGER_H
