#include "serialcomms.h"

SerialComms::SerialComms(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead, this, &SerialComms::readData);
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialComms::handleError);
}

SerialComms::~SerialComms()
{
    if (serial->isOpen())
        serial->close();

    delete serial;
}

QString SerialComms::getOutgoingData() const
{
    return outgoingData;
}

void SerialComms::setOutgoingData(const QString &data)
{
    if (outgoingData != data) {
        outgoingData = data;
        emit outgoingDataChanged();
    }
}

QString SerialComms::getIncomingData() const
{
    QString newData = serial->readAll();

    if (newData != incomingData) {
        incomingData = newData;
        emit incomingDataChanged();
    }

    return incomingData;
}

QString SerialComms::getError() const
{
    return error;
}


void SerialComms::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        this->error = serial->errorString();
        emit errorChanged();

        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());

    }
}

void SerialComms::openSerialPort() {
    const SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        resetAxes();
        resetCTL();
        CTLResetTimeOut = 2500ms / timer->interval();
        (DEBUG_MODE) ? MainStateMachine.setState(IDLE) : MainStateMachine.setState(STARTUP);
        Logger::init();
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        showStatusMessage(tr("Open Port error"));
    }
}
