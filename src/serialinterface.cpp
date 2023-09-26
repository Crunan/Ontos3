#include "serialinterface.h"
#include "logger.h"

const int AUX_INPUT_BUFFER_MAX_SIZE = 90;
const int SERIAL_RESPONSE_TIMEOUT = 5000; // timeout waiting for response (milliseconds)
const int SERIAL_WRITE_TIMEOUT = 1000; // timeout waiting for control pcb response (milliseconds)


SerialInterface::SerialInterface() :
    m_lastSerialCommand(""),
    m_serialPort(),
    m_serialWatchdogTriggered(false)
{
    // serial watchdog timer
    m_pSerialWatchdogTimer = new QTimer(this);
    connect(m_pSerialWatchdogTimer, SIGNAL(timeout()), this, SLOT(serialWatchdogTimerElapsed()));
}

SerialInterface::~SerialInterface()
{
    m_serialPort.close();
    delete m_pSerialWatchdogTimer;
}

void SerialInterface::serialWatchdogTimerElapsed()
{
    m_serialWatchdogTriggered = true;
    m_pSerialWatchdogTimer->stop();
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

        // record command for logging purposes
        m_lastSerialCommand = command;

        Logger::logDebug("AxesController::sendCommand: " + command);

        return true;
    }

    return false;
}

// read a single response
QString SerialInterface::readResponse()
{
    int responseVal = 0;

    QString serialResponse = "";

    // wait up to 100ms for data to be ready
    m_serialPort.waitForReadyRead(250);

    while (responseVal != QChar('#').toLatin1()) {

        responseVal = ReadChar();

        // if the response was zero give it more time for the data to arrive
        while (responseVal == 0) {
            // check the watchdog timer
            if (m_serialWatchdogTriggered) {
                m_serialWatchdogTriggered = false;
                m_pSerialWatchdogTimer->stop();
                Logger::logDebug("AxesController::readResponse() watchdog triggered....bailing");
                Logger::logInfo("Comms error: " + m_lastSerialCommand);
                return 0; // get out if timed out
            }
            m_serialPort.waitForReadyRead(100);
            responseVal = ReadChar();
        }

        serialResponse += char(responseVal);

        // response received so stop the watchdog timer
       m_pSerialWatchdogTimer->stop();
    }

    Logger::logDebug("SerialResponse = " + serialResponse);

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
