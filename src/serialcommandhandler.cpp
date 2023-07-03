#include "serialcommandhandler.h"

SerialCommandHandler::SerialCommandHandler(QSerialPort& serialPort, QObject *parent)
    : QObject(parent), serial(serialPort)
{

}

SerialCommandHandler::~SerialCommandHandler()
{
    // Destructor implementation
    // Perform necessary cleanup or resource deallocation
}

void SerialCommandHandler::sendSerialCommand(const QVariant& data)
{
    setOutgoingData(data);

    QByteArray byteArrayData = outgoingData_.toUtf8(); // Convert QString to QByteArray

    if (serial.isOpen())
    {
        // Write the data to the serial port
        serial.write(byteArrayData);
        serial.flush();
        // handle any errors or perform additional operations here

        // Emit signal to indicate outgoing data is was written.
        emit outgoingDataWritten(byteArrayData);
    }
}

QString SerialCommandHandler::getOutgoingData() const
{
    return outgoingData_;
}

QString SerialCommandHandler::getIncomingData()
{
    QByteArray newData = serial.readAll();

    setIncomingData(newData);

    return QString(incomingData_);
}

void SerialCommandHandler::setOutgoingData(const QVariant& data)
{
    // Convert the data to QString
    QString outgoingData_ = data.toString();

    // Emit signal to indicate outgoing data is ready
    emit outgoingDataReady(outgoingData_);
}

void SerialCommandHandler::setIncomingData(const QString& data)
{
    // Set the value of the outgoingData_ member variable
    incomingData_ = data;

    // Emit a signal to indicate that the outgoing data has changed
    emit incomingDataChanged();
}


