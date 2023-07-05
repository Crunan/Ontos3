#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

class FileReader

{
public:
    FileReader();

    QString getFilePath() const;
    void setFilePath(QString path);

    QString getFileName() const;
    void setFileName(QString fileName);

private:
    QString filePath_;
    QString fileName_;
};

#endif // FILEREADER_H
