#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>

class FileWriter
{
public:
    static void write(const QString& data, const QString& filePath);
};

#endif // FILEWRITER_H
