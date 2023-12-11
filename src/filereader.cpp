#include "filereader.h"

#include <QTextStream>
#include <QDebug>
#include <QFile>

QString FileReader::read(const QString filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file: " << file.errorString();
        return QString();
    }

    QTextStream in(&file);
    QString fileContents = in.readAll();

    file.close();

    return fileContents;
}
