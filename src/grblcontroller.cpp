#include "include/grblcontroller.h"

GRBLController::GRBLController(QObject* parent)
    : QObject(parent)
{    
}

bool GRBLController::open(const SettingsDialog& settings)
{
    const SettingsDialog::Settings p = settings.settings();
    serialPort_.setPortName(p.name);
    serialPort_.setBaudRate(p.baudRate);
    serialPort_.setDataBits(p.dataBits);
    serialPort_.setParity(p.parity);
    serialPort_.setStopBits(p.stopBits);
    serialPort_.setFlowControl(p.flowControl);
    if (!serialPort_.open(QIODevice::ReadWrite)) {
        // Failed to open the serial port
        return false;
    }    
    // Signals connections
    connect(&serialPort_, &QSerialPort::readyRead, this, &GRBLController::readData);
    emit stagePortOpened();

    return true;
}

void GRBLController::close()
{
    serialPort_.close();
}

bool GRBLController::sendCommand(const QString& command)
{
    if (serialPort_.isOpen()) {
        QByteArray requestData = command.toUtf8();
        serialPort_.write(requestData);
        return true;
    }

    return false;
}

QString GRBLController::readData()
{
    QByteArray responseData = serialPort_.readAll();
    QString response = QString::fromUtf8(responseData);
    return response;
}

QString GRBLController::getPortErrorString()
{
    return serialPort_.errorString();
}

bool GRBLController::isOpen()
{
    return serialPort_.isOpen();
}
