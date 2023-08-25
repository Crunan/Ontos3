#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    SerialInterface();
    ~SerialInterface();

    void setPortName(QString portName) { serialPort_.setPortName(portName); }
    void setBaudRate(qint32 baudRate) { serialPort_.setBaudRate(baudRate); }
    void setDataBits(QSerialPort::DataBits dataBits) { serialPort_.setDataBits(dataBits); }
    void setParity(QSerialPort::Parity parity) { serialPort_.setParity(parity); }
    void setStopBits(QSerialPort::StopBits stopBits) { serialPort_.setStopBits(stopBits); }
    void setFlowControl(QSerialPort::FlowControl flowControl) { serialPort_.setFlowControl(flowControl); }
    QString getPortErrorString() { return serialPort_.errorString(); }

    bool open(QIODeviceBase::OpenMode openMode) { return serialPort_.open(openMode); }
    void close() { serialPort_.close(); }
    bool isOpen() { return serialPort_.isOpen(); }
    QString errorString() { return serialPort_.errorString(); }
    bool sendCommand(QString command);
    QString readResponse();

private slots:
    void serialWatchdogTimerElapsed();

private:
    int ReadChar();

    QSerialPort serialPort_;
    bool serialWatchdogTriggered;
    QTimer* pSerialWatchdogTimer;
};

#endif // SERIALINTERFACE_H

