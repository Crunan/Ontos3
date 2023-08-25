#ifndef GRBLCONTROLLER_H
#define GRBLCONTROLLER_H

#include "include/settingsdialog.h"
#include "include/serialinterface.h"
#include <QObject>


class GRBLController : public QObject {
    Q_OBJECT

public:
    explicit GRBLController(QObject* parent = nullptr);
    bool open(const SettingsDialog& settings);
    void close();
    bool sendCommand(const QString& command);
    QString readResponse();

    QString getPortErrorString();
    bool isOpen();
signals:
    void responseReceived(const QString& response);
    void stagePortOpened();

private:
    SerialInterface serialInterface;
};

#endif // GRBLCONTROLLER_H
