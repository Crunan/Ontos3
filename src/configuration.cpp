#include "configuration.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

Configuration::Configuration(QObject *parent) : QObject(parent)
{
    // Get the build directory path
    QString buildDir = QCoreApplication::applicationDirPath();

    // Set the execonfigPath to the "config" folder
    setExeConfigPath(CONFIG_DIRECTORY_NAME);

    // Set the execonfigFileName to the desired file name
    setExeConfigFileName(CONFIG_DEFAULT_FILENAME);

    // Read the configuration file and populate the dictionary
    readConfigFile();
}
// TODO: Once FileReader is implemented, refactor here!
void Configuration::readConfigFile()
{
    // Open the configuration file for reading
    QString filePath = getExeConfigPath() + "/" + getExeConfigFileName();
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle the error if the file cannot be opened
        return;
        emit configFileError();
    }

    // Read the file line by line and populate the dictionary
    QTextStream in(&configFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.at(0) == '/' && line.at(1) == '/') continue; // comment so skip it
        QStringList parts = line.split('>');
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            // remove the '<'
            key = key.removeFirst();
            QString value = parts[1].trimmed();
            m_configDictionary[key] = value;
        }
    }

    // Close the configuration file
    configFile.close();
}
QString Configuration::getValueForKey(const QString& key) const
{
    if (m_configDictionary.contains(key)) {
        return m_configDictionary.value(key);
    }

    // Return an empty string if the key is not found
    return QString();
}

QStringList Configuration::getAllKeys() const
{
    return m_configDictionary.keys();
}

QString Configuration::getExeConfigPath() const
{
    return configFile.getFilePath();
}

QString Configuration::getExeConfigFileName() const
{
    return configFile.getFileName();
}

void Configuration::setExeConfigPath(const QString& path)
{
    configFile.setFilePath(path);
}

void Configuration::setExeConfigFileName(const QString& fileName)
{
    configFile.setFileName(fileName);
}
