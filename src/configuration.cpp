#include "configuration.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

Configuration::Configuration()
{
    // Get the build directory path
    QString buildDir = QCoreApplication::applicationDirPath();

    // Set the execonfigPath to the "config" folder in the build directory
    execonfigPath = buildDir + "/config";

    // Set the execonfigFileName to the desired file name
    execonfigFileName = "config.ini";

    // Read the configuration file and populate the dictionary
    readConfigFile();
}

void Configuration::readConfigFile()
{
    // Open the configuration file for reading
    QString filePath = execonfigPath + "/" + execonfigFileName;
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handle the error if the file cannot be opened
        return;
        emit configFileError();
    }

    // Read the file line by line and populate the dictionary
    QTextStream in(&configFile);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split('=');
        if (parts.size() == 2)
        {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            configDictionary[key] = value;
        }
    }

    // Close the configuration file
    configFile.close();
}
QString Configuration::getValueForKey(const QString& key) const
{
    if (configDictionary.contains(key))
    {
        return configDictionary.value(key);
    }

    // Return an empty string if the key is not found
    return QString();
}

QStringList Configuration::getAllKeys() const
{
    return configDictionary.keys();
}

QString Configuration::getExeConfigPath() const
{
    return execonfigPath;
}

QString Configuration::getExeConfigFileName() const
{
    return execonfigFileName;
}
