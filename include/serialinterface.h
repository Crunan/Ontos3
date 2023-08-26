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

    void setPortName(QString portName) { m_serialPort.setPortName(portName); }
    void setBaudRate(qint32 baudRate) { m_serialPort.setBaudRate(baudRate); }
    void setDataBits(QSerialPort::DataBits dataBits) { m_serialPort.setDataBits(dataBits); }
    void setParity(QSerialPort::Parity parity) { m_serialPort.setParity(parity); }
    void setStopBits(QSerialPort::StopBits stopBits) { m_serialPort.setStopBits(stopBits); }
    void setFlowControl(QSerialPort::FlowControl flowControl) { m_serialPort.setFlowControl(flowControl); }
    QString getPortErrorString() { return m_serialPort.errorString(); }

    bool open(QIODeviceBase::OpenMode openMode) { return m_serialPort.open(openMode); }
    void close() { m_serialPort.close(); }
    bool isOpen() { return m_serialPort.isOpen(); }
    QString errorString() { return m_serialPort.errorString(); }
    bool sendCommand(QString command);
    QString readResponse();

private slots:
    void serialWatchdogTimerElapsed();

private:
    int ReadChar();

    QSerialPort m_serialPort;
    bool m_serialWatchdogTriggered;
    QTimer* m_pSerialWatchdogTimer;
};

#endif // SERIALINTERFACE_H

