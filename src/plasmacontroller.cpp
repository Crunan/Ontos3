#include "plasmacontroller.h"

PlasmaController::PlasmaController(QWidget* parent)
  : QObject(parent),
    plasmaHead(),
    pwr(),
    tuner(),
    mfcs({ new MFC(1), new MFC(2), new MFC(3), new MFC(4) }),
    commandMap(),
    config(),
    axesCTL(nullptr),
    serialPort_(),
    ledStatus(),
    executeRecipe(0)
{
    // Add startup data gathering methods.
    for (MFC* mfc: mfcs) {
        connect(mfc, &MFC::defaultRecipeChanged, this, &PlasmaController::handleSetMFCDefaultRecipeCommand);
        connect(mfc, &MFC::recipeFlowChanged, this, &PlasmaController::handleSetMFCRecipeFlowCommand);
        connect(mfc, &MFC::rangeChanged, this, &PlasmaController::handleSetMFCRangeCommand);
    }
    connect(&tuner, &Tuner::defaultRecipeChanged, this, &PlasmaController::handleSetTunerDefaultRecipeCommand);
    connect(&tuner, &Tuner::recipePositionChanged, this, &PlasmaController::handleSetTunerRecipePositionCommand);
    connect(&tuner, &Tuner::autoTuneChanged, this, &PlasmaController::handleSetTunerAutoTuneCommand);
}

PlasmaController::~PlasmaController()
{
    // Clean up the MFC objects
    for (MFC* mfc : mfcs) {
        delete mfc;
    }
}

bool PlasmaController::open(const SettingsDialog& settings)
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
    connect(&serialPort_, &QSerialPort::readyRead, this, &PlasmaController::readData);
    emit mainPortOpened();

    return true;
}

void PlasmaController::close()
{
    serialPort_.close();
}


bool PlasmaController::sendCommand(const QString& command)
{
    if (serialPort_.isOpen()) {
        QByteArray requestData = command.toUtf8();
        serialPort_.write(requestData);
        return true;
    }

    return false;
}

QString PlasmaController::readData()
{
    QByteArray responseData = serialPort_.readAll();
    QString response = QString::fromUtf8(responseData);
    return response;
}

void PlasmaController::setCommandMap(const QMap<QString, QPair<QString, QString>>& map)
{
    commandMap.setCommandMap(map);
}
QString PlasmaController::findCommandValue(QString command) const
{
    return commandMap.findCommandValue(command);
}


QString PlasmaController::formatSerialCommand(QString cmd, const QString& setpoint)
{
    // Remove the trailing '%' character
    cmd.chop(1);

    // Add the setpoint to the command
    cmd += setpoint;

    // Add the '%' character back to the command
    cmd += "%";

    return cmd;
}

MFC* PlasmaController::findMFCByNumber(int& mfcNumber)
{
    for (MFC* mfc : mfcs) {
        if (mfc->getMFCNumber() == mfcNumber) {
            return mfc;
        }
    }
    return nullptr;
}

int PlasmaController::numberOfMFCs()
{
//    QString command = "$30%";
//    send(command);
//    int numMFCs = parseResponseForNumberOfMFCs(response);

//    return numMFCs;
}

int PlasmaController::parseResponseForNumberOfMFCs(QString& response)
{
    // _ $30%; resp[!300m#], m = number of MFCs
    int numMFCIndex = 5;

    // Extract the number of MFCs substring
    QString numMFCsStr = response.mid(1, numMFCIndex - 1);

    // Convert the MFC number to an integer
    int numMFCs = numMFCsStr.toInt();

    // Output the extracted data (for demonstration purposes)
    qDebug() << "Number of MFC's:" << numMFCs;

    return numMFCs;
}

QString PlasmaController::getPortErrorString()
{
    return serialPort_.errorString();
}

bool PlasmaController::isOpen()
{
    return serialPort_.isOpen();
}

double PlasmaController::handleGetMFCRecipeFlowCommand(QString& responseStr)
{
    int mfcIndex = 5;
    int flowIndex = 6;
    // Extract the MFC number substring ('m')
    QString mfcNumberStr = responseStr.mid(1, mfcIndex - 1);

    // Extract the flow substring (characters after 'm')
    QString flowStr = responseStr.mid(4, flowIndex - 1);

    // Convert the MFC number and flow to integers
    int mfcNumber = mfcNumberStr.toInt();
    int flow = flowStr.toInt();

    // Going to set the mfc data, find which mfc is updated
    MFC* mfc = findMFCByNumber(mfcNumber);

    // Set the data in the mfc structure
    mfc->setRecipeFlow(flow);

    // Output the extracted data (for demonstration purposes)
    qDebug() << "MFC Number:" << mfcNumber;
    qDebug() << "Flow:" << flow;

    // Call the getter for the mfc structure
    return mfc->getRecipeFlow();
}

// MFC
void PlasmaController::handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow)
{

}
void PlasmaController::handleSetMFCDefaultRecipeCommand(const int mfcNumber, const double recipeFlow)
{

}
void PlasmaController::handleSetMFCRangeCommand(const int mfcNumber, const double range)
{

}
// TUNER
void PlasmaController::handleSetTunerRecipePositionCommand(const double recipePosition)
{
    QString command = "$43" + QString::number(recipePosition) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::handleSetTunerDefaultRecipeCommand(const double defaultPosition)
{
    QString command = "$2A606" + QString::number(defaultPosition) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::handleSetTunerAutoTuneCommand(const bool value)
{
    QString command = "$860" + QString::number(value) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::handleSetPWRDefaultRecipeCommand(const double defaultWatts)
{
    QString command = "$2A605" + QString::number(defaultWatts) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::handleSetPWRRecipeWattsCommand(const double recipeWatts)
{
    QString command = "$42" + QString::number(recipeWatts) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::handleSetPWRMaxWattsCommand(const double maxWatts)
{
    QString command = "$2A705" + QString::number(maxWatts) + "%";
    //serial.commandHandler.send(command);
}

void PlasmaController::getCTLStatusCommand()
{
//    QString command = "$91%";
//    QString response = send(command);
//    parseResponseForCTLStatus(response);

}


void PlasmaController::setLEDStatus(int &bits)
{
    ledStatus.setStatusBits(bits);
}

void PlasmaController::parseResponseForCTLStatus(const QString& response)
{    
    // Extract LED status
    QString ledStatusHex = response.mid(3, 4); // Assuming LLRR are at positions 3 to 6
    bool ok;
    int ledStatus = ledStatusHex.toInt(&ok, 16);
    if (ok) {
        // TODO: Call setter for LED status with the parsed integer value
        setLEDStatus(ledStatus);
    } else {
        // Handle error if the conversion fails
    }

    // Split the response string into subsystem data
    QStringList subsystemData = response.mid(6, response.length() - 7).split(";");

    // Check if we have enough data to update the subsystems
    if (subsystemData.size() != 10) {
        // Handle error or return if there is not enough data
        return;
    }

    // Extract and update the tuner position
    double tunerPosition = subsystemData[1].toDouble();
    tuner.setActualPosition(tunerPosition);

    // Extract and update RF power forward and reflected
    double rfForward = subsystemData[2].toDouble();
    double rfReflected = subsystemData[3].toDouble();
    pwr.setForwardWatts(rfForward);
    pwr.setReflectedWatts(rfReflected);

    // Extract and update ExecuteRecipe member
    int execRecipeInt = subsystemData[4].toInt();
    bool execRecipe = (execRecipeInt != 0);
    setExecuteRecipe(execRecipe);

    // Extract and update MFC actual flow values
    for (int i = 1; i <= 4; i++) {
        MFC* mfc = findMFCByNumber(i);
        double mfcFlow = subsystemData[4 + i].toDouble();
        mfc->setActualFlow(mfcFlow);

    }

    // Extract and update plasmahead temperature
    double temperature = subsystemData[9].toDouble();
    plasmaHead.setTemperature(temperature);
}

bool PlasmaController::getExecuteRecipe() const
{
    return executeRecipe;
}

void PlasmaController::setExecuteRecipe(bool value)
{
    executeRecipe = value;
}



void PlasmaController::CTLStartup()
{
//    howManyMFCs();
//    getBatchIDLogging();
//    getRecipeMBPosition();
//    getRecipeRFPosition();
//    getRecipeMFC4Flow();
//    getRecipeMFC3Flow();
//    getRecipeMFC2Flow();
//    getRecipeMFC1Flow();
//    getMFC4Range();
//    getMFC3Range();
//    getMFC2Range();
//    getMFC1Range();
//    getMaxRFPowerForward();
//    getAutoMan();
//    turnOffExecRecipe();
//    getTemp();
//    setCTLStateMachinesIdle();
}


//void MainWindow::UpdateStatus() {
//    didStatusBitsChange();
//    StatusBitsWas = StatusBits;

//    (StatusBits & 0x0100) > 0 ? ui->actionGAS_1->setChecked(true) : ui->actionGAS_1->setChecked(false);
//    (StatusBits & 0x0200) > 0 ? ui->actionGAS_2->setChecked(true) : ui->actionGAS_2->setChecked(false);
//    (StatusBits & 0x0400) > 0 ? ui->actionGAS_3->setChecked(true) : ui->actionGAS_3->setChecked(false);
//    (StatusBits & 0x0800) > 0 ? ui->actionGAS_4->setChecked(true) : ui->actionGAS_4->setChecked(false);

//    (StatusBits & 0x1000) > 0 ? ui->actionV5->setChecked(true) : ui->actionV5->setChecked(false);
//    (StatusBits & 0x2000) > 0 ? ui->actionV6->setChecked(true) : ui->actionV6->setChecked(false);
//    (StatusBits & 0x4000) > 0 ? ui->actionV7->setChecked(true) : ui->actionV7->setChecked(false);
//    (StatusBits & 0x8000) > 0 ? ui->actionRF_ENABLED->setChecked(true) : ui->actionRF_ENABLED->setChecked(false);

//    (StatusBits & 0x0001) > 0 ? ui->actionPLASMA_ON->setChecked(true) : ui->actionPLASMA_ON->setChecked(false);
//    (StatusBits & 0x0002) > 0 ? ui->actionTUNING->setChecked(true) : ui->actionTUNING->setChecked(false);
//    (StatusBits & 0x0004) > 0 ? ui->actionAUTO_MODE->setChecked(true) : ui->actionAUTO_MODE->setChecked(false);
//    (StatusBits & 0x0008) > 0 ? ui->actionEXECUTE_RECIPE->setChecked(true) : ui->actionEXECUTE_RECIPE->setChecked(false);

//    (StatusBits & 0x0010) > 0 ? ui->actionESTOP_ON->setChecked(true) : ui->actionESTOP_ON->setChecked(false);
//    (StatusBits & 0x0020) > 0 ? ui->actionDO_CMD->setChecked(true) : ui->actionDO_CMD->setChecked(false);
//    (StatusBits & 0x0040) > 0 ? ui->actionHE_SIG->setChecked(true) : ui->actionHE_SIG->setChecked(false);
//    (StatusBits & 0x0080) > 0 ? ui->actionPROCESS_ABORT->setChecked(true) : ui->actionPROCESS_ABORT->setChecked(false);

//    if (ui->actionEXECUTE_RECIPE->isChecked()) {
//        RunRecipeOn = true;
//    }
//    else {
//        RunRecipeOn= false;
//    }

//}
