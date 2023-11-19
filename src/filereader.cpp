#include "filereader.h"


FileReader::FileReader()
    : m_filePath(""), m_fileName("")
{
}

QString FileReader::getFilePath() const
{
    return m_filePath;
}

void FileReader::setFilePath(QString path)
{
    m_filePath = path;
}

QString FileReader::getFileName() const
{
    return m_fileName;
}

void FileReader::setFileName(QString fileName)
{
    m_fileName = fileName;
}
