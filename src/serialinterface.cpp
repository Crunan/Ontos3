#include "serialinterface.h"
#include "logger.h"

const int AUX_INPUT_BUFFER_MAX_SIZE = 90;
const int SERIAL_RESPONSE_TIMEOUT = 5000; // timeout waiting for response (milliseconds)
const int SERIAL_WRITE_TIMEOUT = 1000; // timeout waiting for control pcb response (milliseconds)

SerialInterface::SerialInterface()
{
    // serial watchdog timer
    pSerialWatchdogTimer = new QTimer(this);
    connect(pSerialWatchdogTimer, SIGNAL(timeout()), this, SLOT(serialWatchdogTimerElapsed()));

    serialWatchdogTriggered = false;
}

SerialInterface::~SerialInterface()
{
    delete pSerialWatchdogTimer;
}

void SerialInterface::serialWatchdogTimerElapsed()
{
    serialWatchdogTriggered = true;
    pSerialWatchdogTimer->stop();
}

bool SerialInterface::sendCommand(QString command)
{
    int commandLength = command.length();

    // don't overrun CTL input buffers.  Garbage protection
    if (commandLength> AUX_INPUT_BUFFER_MAX_SIZE) commandLength = AUX_INPUT_BUFFER_MAX_SIZE;

    if (serialPort_.isOpen()) {
        QByteArray sendData = command.toUtf8();

        serialPort_.write(command.toLatin1(), commandLength);

        // wait for bytes to be written
        serialPort_.waitForBytesWritten(SERIAL_WRITE_TIMEOUT);

        // start watchdog timer
        pSerialWatchdogTimer->start(SERIAL_RESPONSE_TIMEOUT);

        Logger::logInfo("AxesController::sendCommand: " + command);

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
    serialPort_.waitForReadyRead(250);

    while (responseVal != QChar('#').toLatin1()) {

        responseVal = ReadChar();

        // if the response was zero give it more time for the data to arrive
        while (responseVal == 0) {
            // check the watchdog timer
            if (serialWatchdogTriggered) {
                serialWatchdogTriggered = false;
                pSerialWatchdogTimer->stop();
                Logger::logCritical("AxesController::readResponse() watchdog triggered....bailing");
                return 0; // get out if timed out
            }
            serialPort_.waitForReadyRead(100);
            responseVal = ReadChar();
        }

        serialResponse += char(responseVal);

        // response received so stop the watchdog timer
        pSerialWatchdogTimer->stop();
    }

    Logger::logDebug("SerialResponse = " + serialResponse);

    return serialResponse;
}

int SerialInterface::ReadChar()
{
    int returnValue = 0;
    char readChar;

    if (serialPort_.isOpen()) {
        returnValue = serialPort_.read(&readChar, 1);

        if (returnValue == -1) {
            Logger::logCritical("Error: Reading from serialPort_" );
            return returnValue;
        }
        else if (returnValue == 0) { // no more data
            return returnValue;
        }
    }

    return readChar;
}
