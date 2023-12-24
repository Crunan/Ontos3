#include "serialinterface.h"
#include "logger.h"
#include <QApplication>

const int AUX_INPUT_BUFFER_MAX_SIZE = 90;
const int SERIAL_RESPONSE_TIMEOUT = 3000; // timeout waiting for response (milliseconds)
const int SERIAL_WRITE_TIMEOUT = 1000; // timeout waiting for control pcb response (milliseconds)

SerialInterface::SerialInterface() :
    m_lastSerialCommand(""),
    m_serialPort(),
    m_serialWatchdogTriggered(false),
    m_done(false)
{
    // serial watchdog timer
    m_pSerialWatchdogTimer = new QTimer(this);
    connect(m_pSerialWatchdogTimer, &QTimer::timeout, this, &SerialInterface::serialWatchdogTimerElapsed);
}

SerialInterface::~SerialInterface()
{
    m_done = true;
    m_serialPort.close();
    delete m_pSerialWatchdogTimer;
}

bool SerialInterface::open(QIODeviceBase::OpenMode openMode)
{
    bool portOpen = m_serialPort.open(openMode);
    if (portOpen) {
        emit serialOpen();
    }
    return portOpen;
}

void SerialInterface::close()
{
    emit serialClosed();
    m_serialPort.close();
}


void SerialInterface::serialWatchdogTimerElapsed()
{
    m_serialWatchdogTriggered = true;
    m_pSerialWatchdogTimer->stop();
    m_serialPort.close(); // somethings obviously wrong so close the serial port
    emit serialClosed();
    emit readTimeoutError(m_lastSerialCommand);
}

bool SerialInterface::sendCommand(QString command)
{
    int commandLength = command.length();

    // don't overrun CTL input buffers.  Garbage protection
    if (commandLength> AUX_INPUT_BUFFER_MAX_SIZE) commandLength = AUX_INPUT_BUFFER_MAX_SIZE;

    if (m_serialPort.isOpen()) {
        QByteArray sendData = command.toUtf8();

        m_serialPort.write(command.toLatin1(), commandLength);

        // wait for bytes to be written
        m_serialPort.waitForBytesWritten(SERIAL_WRITE_TIMEOUT);

        // start watchdog timer
        m_pSerialWatchdogTimer->start(SERIAL_RESPONSE_TIMEOUT);
        m_serialWatchdogTriggered = false;

        // record command for logging purposes
        m_lastSerialCommand = command;

        Logger::logDebug("sendCommand: " + command);

        return true;
    }

    return false; // consider using the return value
}

// read a single response
QString SerialInterface::readResponse()
{
    int responseVal = 0;

    QString serialResponse = "";

    if (m_serialPort.isOpen()) {

        // wait up to 100ms for data to be ready
        m_serialPort.waitForReadyRead(250);

        while (responseVal != QChar('#').toLatin1() && !m_done) {

            responseVal = ReadChar();

            // if the response was zero give it more time for the data to arrive
            while (responseVal == 0) {
                // check the watchdog timer
                if (m_serialWatchdogTriggered) {
                    m_pSerialWatchdogTimer->stop();
                    Logger::logDebug("readResponse() watchdog triggered....bailing");
                    Logger::logInfo("Comms error: " + m_lastSerialCommand);
                    return 0; // get out if timed out
                }
                m_serialPort.waitForReadyRead(100);
                responseVal = ReadChar();
                qApp->processEvents(); // allow the timer event to be serivced if the board is not present
            }

            serialResponse += char(responseVal);

            // response received so stop the watchdog timer
           m_pSerialWatchdogTimer->stop();
        }

        Logger::logDebug("SerialResponse = " + serialResponse);
    }

    return serialResponse;
}

int SerialInterface::ReadChar()
{
    int returnValue = 0;
    char readChar;

    if (m_serialPort.isOpen()) {
        returnValue = m_serialPort.read(&readChar, 1);

        if (returnValue == -1) {
            Logger::logDebug("Error: Reading from m_serialPort" );
            return returnValue;
        }
        else if (returnValue == 0) { // no more data
            return returnValue;
        }
    }

    return readChar;
}
