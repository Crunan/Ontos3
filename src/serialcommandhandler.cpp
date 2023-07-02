#include "serialcommandhandler.h"

SerialCommandHandler::SerialCommandHandler(QSerialPort& serialPort, QObject *parent)
    : QObject(parent), serial(serialPort)
{
    connect(this, &SerialCommandHandler::outgoingDataChanged, this, &SerialCommandHandler::writeOutgoingData);
}

SerialCommandHandler::~SerialCommandHandler()
{
    // Destructor implementation
    // Perform necessary cleanup or resource deallocation
}

QString SerialCommandHandler::getOutgoingData() const
{
    return outgoingData_;
}

void SerialCommandHandler::setOutgoingData(const QVariant &data)
{
    // Convert the data to QString
    QString convertedData = data.toString();

    if (outgoingData_ != convertedData) {
        outgoingData_ = convertedData;
        emit outgoingDataChanged();
    }
}

void SerialCommandHandler::writeOutgoingData()
{
    QByteArray data = outgoingData_.toUtf8(); // Convert QString to QByteArray

    if (serial.isOpen())
    {
        // Write the data to the serial port
        serial.write(data);
        serial.flush();
        // handle any errors or perform additional operations here
    }
}


QString SerialCommandHandler::getIncomingData() {
    QByteArray newData = serial.readAll();

    if (newData != incomingData_) {
        incomingData_ = newData;
        emit incomingDataChanged();
    }

    return QString(incomingData_);
}
