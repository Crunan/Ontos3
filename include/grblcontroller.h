#ifndef GRBLCONTROLLER_H
#define GRBLCONTROLLER_H

#include <QObject>
#include <QSerialPort>

class GRBLController : public QObject {
    Q_OBJECT

public:
    explicit GRBLController(QObject* parent = nullptr);
    bool open(const QString& portName);
    void close();
    bool sendCommand(const QString& command);

signals:
    void responseReceived(const QString& response);

private slots:
    void readData();

private:
    QSerialPort serialPort;
};

#endif // GRBLCONTROLLER_H
