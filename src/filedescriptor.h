#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include <QString>


class FileDescriptor

{
public:
    FileDescriptor();

    const QString getFilePath() const;
    void setFilePath(QString path);

    const QString getFileName() const;
    void setFileName(QString fileName);

private:
    QString filePath;
    QString fileName;
};

#endif // FILEDESCRIPTOR_H
