#include "filedescriptor.h"


FileDescriptor::FileDescriptor()
    : filePath(""), fileName("")
{
}

const QString FileDescriptor::getFilePath() const
{
    return filePath;
}

void FileDescriptor::setFilePath(QString path)
{
    filePath = path;
}

const QString FileDescriptor::getFileName() const
{
    return fileName;
}

void FileDescriptor::setFileName(QString fileName)
{
    fileName = fileName;
}
