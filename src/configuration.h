#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QCoreApplication>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QObject>

const QString CONFIG_DIRECTORY_NAME = "/opt/OTT_PLUS/config";
const QString CONFIG_DEFAULT_FILENAME = "default.cfg";
const QString CONFIG_MFC1_LABEL_KEY = "MFC_LABEL_1";
const QString CONFIG_MFC2_LABEL_KEY = "MFC_LABEL_2";
const QString CONFIG_MFC3_LABEL_KEY = "MFC_LABEL_3";
const QString CONFIG_MFC4_LABEL_KEY = "MFC_LABEL_4";
const QString CONFIG_COM_PORT_KEY = "SERIAL_PORT";
const QString CONFIG_PASSWORD_KEY = "PW";
// axis CTL
const QString CONFIG_DOOR_STATUS_BIT = "DOOR_STATUS_BIT";
const QString CONFIG_N2PURGE_STATUS_BIT = "N2_PURGE_STATUS_BIT";
const QString CONFIG_VAC_STATUS_BIT = "VAC_STATUS_BIT";
const QString CONFIG_JOY_STATUS_BIT = "JOY_STATUS_BIT";
// OTT CTL
const QString CONFIG_PLASMA_STATUS_BIT = "PLASMA_STATUS_BIT";
const QString CONFIG_ESTOP_STATUS_BIT = "ESTOP_STATUS_BIT";
const QString CONFIG_ABORT_STATUS_BIT = "ABORT_STATUS_BIT";

class Configuration : public QObject
{
public:
    Configuration(QObject *parent = nullptr);

    QString getExeConfigPath() const;
    QString getExeConfigFileName() const;
    QString getValueForKey(const QString& key) const;
    QStringList getAllKeys() const;

signals:
    void configFileError();

private:
    void readConfigFile();

    QString m_execonfigPath;
    QString m_execonfigFileName;
    QMap<QString, QString> m_configDictionary;
};

#endif // CONFIGURATION_H
