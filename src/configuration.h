#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QCoreApplication>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QObject>

static const char CONFIG_DIRECTORY_NAME[] = "/opt/OTT_PLUS/config";
static const char CONFIG_DEFAULT_FILENAME[] = "default.cfg";
static const char CONFIG_MFC1_LABEL_KEY[] = "MFC_LABEL_1";
static const char CONFIG_MFC2_LABEL_KEY[] = "MFC_LABEL_2";
static const char CONFIG_MFC3_LABEL_KEY[] = "MFC_LABEL_3";
static const char CONFIG_MFC4_LABEL_KEY[] = "MFC_LABEL_4";
static const char CONFIG_MFC5_LABEL_KEY[] = "MFC_LABEL_5";
static const char CONFIG_MFC6_LABEL_KEY[] = "MFC_LABEL_6";
static const char CONFIG_COM_PORT_KEY[] = "SERIAL_PORT";
static const char CONFIG_PASSWORD_KEY[] = "PW";
static const char HANDSHAKE[] = "HANDSHAKE";
// axis CTL
static const char CONFIG_DOOR_STATUS_BIT[] = "DOOR_STATUS_BIT";
static const char CONFIG_N2PURGE_STATUS_BIT[] = "N2_PURGE_STATUS_BIT";
static const char CONFIG_VAC_STATUS_BIT[] = "VAC_STATUS_BIT";
static const char CONFIG_JOY_STATUS_BIT[] = "JOY_STATUS_BIT";
// OTT CTL
static const char CONFIG_PLASMA_STATUS_BIT[] = "PLASMA_STATUS_BIT";
static const char CONFIG_ESTOP_STATUS_BIT[] = "ESTOP_STATUS_BIT";
static const char CONFIG_ABORT_STATUS_BIT[] = "ABORT_STATUS_BIT";

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
