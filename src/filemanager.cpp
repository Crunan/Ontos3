#include "filemanager.h"

FileManager::FileManager(FileWriter& writer, FileDescriptor& metaData, FileReader &fileReader)
    : fileWriter(writer), fileMetaData(metaData), fileReader(fileReader) {

}

void FileManager::write(const QString &data) {
    fileWriter.write(data, getFilePath() + getFileName());
}

const QString FileManager::getFilePath() const {
    return fileMetaData.getFilePath();
}

const QString FileManager::getFileName() const {
    return fileMetaData.getFileName();
}

QString FileManager::read(const QString &fileToRead) const {
    return fileReader.read(fileToRead);
}
