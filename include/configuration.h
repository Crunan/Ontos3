#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QCoreApplication>
#include <QString>
#include <QMap>
#include <QStringList>

class Configuration
{
public:
    Configuration();

    QString getExeConfigPath() const;
    QString getExeConfigFileName() const;
    QString getValueForKey(const QString& key) const;
    QStringList getAllKeys() const;

private:
    void readConfigFile();

    QString execonfigPath;
    QString execonfigFileName;
    QMap<QString, QString> configDictionary;

signals:
    void configFileError();
};

#endif // CONFIGURATION_H
