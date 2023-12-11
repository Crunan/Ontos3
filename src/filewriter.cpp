// filewriter.cpp

#include "filewriter.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

void FileWriter::write(const QString& data, const QString& filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    }
    else {
        qWarning() << "Unable to open the file for writing";
    }
}
