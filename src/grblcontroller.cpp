#include "include/grblcontroller.h"

GRBLController::GRBLController(QObject* parent)
    : QObject(parent)
{    
}

bool GRBLController::open(const SettingsDialog& settings)
{
    const SettingsDialog::Settings p = settings.settings();
    serialInterface.setPortName(p.name);
    serialInterface.setBaudRate(p.baudRate);
    serialInterface.setDataBits(p.dataBits);
    serialInterface.setParity(p.parity);
    serialInterface.setStopBits(p.stopBits);
    serialInterface.setFlowControl(p.flowControl);
    if (!serialInterface.open(QIODevice::ReadWrite)) {
        // Failed to open the serial port
        return false;
    }    
    // Signals connections
    //connect(&serialInterface, &QSerialPort::readyRead, this, &GRBLController::readData);
    emit stagePortOpened();

    return true;
}

void GRBLController::close()
{
    serialInterface.close();
}

bool GRBLController::sendCommand(const QString& command)
{
    return serialInterface.sendCommand(command);
}

QString GRBLController::readResponse()
{
    return serialInterface.readResponse();
}

QString GRBLController::getPortErrorString()
{
    return serialInterface.errorString();
}

bool GRBLController::isOpen()
{
    return serialInterface.isOpen();
}
