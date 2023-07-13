#ifndef GRBLCONTROLLER_H
#define GRBLCONTROLLER_H

#include "include/settingsdialog.h"

#include <QObject>
#include <QSerialPort>

class GRBLController : public QObject {
    Q_OBJECT

public:
    explicit GRBLController(QObject* parent = nullptr);
    bool open(const SettingsDialog& settings);
    void close();
    bool sendCommand(const QString& command);
    QString readData();

    QString getPortErrorString();
    bool isOpen();
signals:
    void responseReceived(const QString& response);
    void stagePortOpened();

private:
    QSerialPort serialPort_;
};

#endif // GRBLCONTROLLER_H
