#include "grblcontroller.h"

GRBLController::GRBLController(QObject* parent)
    : QObject(parent)
{
    serialPort.setBaudRate(115200); // Set the appropriate baud rate
    connect(&serialPort, &QSerialPort::readyRead, this, &GRBLController::readData);
}

bool GRBLController::open(const QString& portName)
{
    serialPort.setPortName(portName);
    if (!serialPort.open(QIODevice::ReadWrite)) {
        // Failed to open the serial port
        return false;
    }

    return true;
}

void GRBLController::close()
{
    serialPort.close();
}

bool GRBLController::sendCommand(const QString& command)
{
    if (serialPort.isOpen()) {
        QByteArray requestData = command.toUtf8();
        serialPort.write(requestData);
        return true;
    }

    return false;
}

void GRBLController::readData()
{
    QByteArray responseData = serialPort.readAll();
    QString response = QString::fromUtf8(responseData);
    emit responseReceived(response);
}
