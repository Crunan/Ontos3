#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>

#include "include/settingsdialog.h"
#include "include/serialcommandhandler.h"

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error_ READ getError CONSTANT)

private:
    QString error_;

public:
    SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    QString getError() const;

signals:
    void serialPortOpened();
    void serialPortClosed();
    void errorOccurred();

public slots:
    void openSerialPort(SettingsDialog::Settings settings);
    void closeSerialPort();
    void handleError();

public:
    QSerialPort serial; // Serial port object
    SerialCommandHandler commandHandler; // Command handler object
};

#endif // SERIALPORTMANAGER_H
