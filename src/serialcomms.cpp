#include "include/serialcomms.h"
SerialComms::SerialComms(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialComms::getIncomingData);
    connect(serial, &QSerialPort::errorOccurred, this, &SerialComms::handleError);
}

SerialComms::~SerialComms() {
    if (serial->isOpen())
        serial->close();

    delete serial;
}

QString SerialComms::getOutgoingData() const
{
    return outgoingData;
}

void SerialComms::writeOutgoingData()
{
    QByteArray data = outgoingData.toUtf8(); // Convert QString to QByteArray

    if (serial && serial->isOpen())
    {
        // Write the data to the serial port
        serial->write(data);

        // handle any errors or perform additional operations here
    }
}
void SerialComms::setOutgoingData(const QVariant &data)
{
    // Convert the data to QString
    QString convertedData = data.toString();

    if (outgoingData != convertedData) {
        outgoingData = convertedData;
        emit outgoingDataChanged();
    }
}

QString SerialComms::getIncomingData() {
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

void SerialComms::openSerialPort(const SettingsDialog::Settings& p) {
    if (serial->isOpen()) {
        return;
    }

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    serial->open(QIODevice::ReadWrite);
    // Serial port opened successfully
    emit serialPortOpened();
}

void SerialComms::handleError() {
    this->error = serial->errorString();
    emit errorOccurred();
}

void SerialComms::closeSerialPort() {
    if (serial->isOpen()) {
//        resetCTL();
//        resetAxes();
        serial->close();
        emit serialPortClosed();
    }
}
