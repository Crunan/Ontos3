#include "include/serialcomms.h"

SerialComms::SerialComms(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialComms::getIncomingData);
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialComms::handleError);
}

SerialComms::~SerialComms() {
    if (serial->isOpen())
        serial->close();

    delete serial;
}

QString SerialComms::getOutgoingData() const {
    return outgoingData;
}

void SerialComms::setOutgoingData(const QString &data) {
    if (outgoingData != data) {
        outgoingData = data;
        emit outgoingDataChanged();
    }
}

QString SerialComms::getIncomingData() const {
    QByteArray newData = serial->readAll();

    if (newData != incomingData) {
        incomingData = newData;
        emit incomingDataChanged();
    }

    return QString(incomingData);
}

QString SerialComms::getError() const {
    return error;
}


void SerialComms::setSerialSettings() {
    const SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
}

void SerialComms::openSerialPort() {
    setSerialSettings();
    serial->open(QIODevice::ReadWrite);
    // Serial port opened successfully
    emit serialPortOpened();
}

void SerialComms::handleError(QSerialPort::SerialPortError error)
{
    // Error handling logic
    if (error == QSerialPort::ResourceError) {
        this->error = serial->errorString();
        emit errorOccured();
    }else if (error == QSerialPort::ReadError) {
        // Handle read error
    } else if (error == QSerialPort::WriteError) {
        // Handle write error
    } else if (error == QSerialPort::TimeoutError) {
        // Handle timeout error
    }
    // ...
}

void SerialComms::closeSerialPort() {
//    MainStateMachine.setState(SHUTDOWN);
//    if (serial->isOpen()) {
//        resetCTL();
//        resetAxes();
//        serial->close();
}

//    ui->actionConnect->setEnabled(true);
//    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
//    logInfo("Port Disconnected");
//    showStatusMessage(tr("Disconnected"));
//}
