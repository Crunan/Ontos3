#include "include/filereader.h"


FileReader::FileReader()
    : filePath_(""), fileName_("")
{
}

QString FileReader::getFilePath() const
{
    return filePath_;
}

void FileReader::setFilePath(QString path)
{
    filePath_ = path;
}

QString FileReader::getFileName() const
{
    return fileName_;
}

void FileReader::setFileName(QString fileName)
{
    fileName_ = fileName;
}
