// filewriter.cpp

#include "filewriter.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

void FileWriter::write(const QString& data, const QString& filePath) {
    QFile file(filePath);
    if (file.isOpen()) { qDebug() << "File is open"; }
    if (file.isWritable()) { qDebug() << "File is Writeable"; }
    if (file.isReadable()) { qDebug() << "File is Readable"; }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
        qDebug() << "File written successfully";
    } else {
        qDebug() << "Failed to open file for writing";
    }
}
