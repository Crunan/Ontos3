#include "include/serialportmanager.h"

SerialPortManager::SerialPortManager(QObject *parent)
  : QObject(parent),
    error_(),
    serial(),
    commandHandler(serial)
{
}

SerialPortManager::~SerialPortManager() {
    if (serial.isOpen())
        serial.close();
}

QString SerialPortManager::getError() const {
    return error_;
}

void SerialPortManager::openSerialPort(SettingsDialog::Settings settings) {
    if (serial.isOpen()) {
        return;
    }

    serial.setPortName(settings.name);
    serial.setBaudRate(settings.baudRate);
    serial.setDataBits(settings.dataBits);
    serial.setParity(settings.parity);
    serial.setStopBits(settings.stopBits);
    serial.setFlowControl(settings.flowControl);

    if (serial.open(QIODevice::ReadWrite))
    {        
        // Serial port opened successfully
        emit serialPortOpened();
    }
}

void SerialPortManager::closeSerialPort()
{
    if (serial.isOpen())
    {
        serial.close();
        emit serialPortClosed();
    }
}

void SerialPortManager::handleError() {
    error_ = serial.errorString();
    emit errorOccurred();
}

