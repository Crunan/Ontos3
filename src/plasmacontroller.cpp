#include "plasmacontroller.h"
#include "logger.h"

PlasmaController::PlasmaController(QWidget* parent)
  : QObject(parent),
    m_plasmaHead(),
    m_pwr(),
    m_tuner(),
    m_mfcs({ new MFC(1), new MFC(2), new MFC(3), new MFC(4) }),
    m_commandMap(),
    m_config(),
    m_pSerialInterface(new SerialInterface()),
    m_ledStatus(),
    m_executeRecipe(0),
    m_numMFCs(0),
    m_readyToLoad(false),
    m_MBtunerRecipeSP(false),
    m_RFtunerRecipeSP(false)
{
    // Add startup data gathering methods.
    for (MFC* mfc: m_mfcs) {
        connect(mfc, &MFC::defaultRecipeChanged, this, &PlasmaController::handleSetMFCDefaultRecipeCommand);
        connect(mfc, &MFC::recipeFlowChanged, this, &PlasmaController::handleSetMFCRecipeFlowCommand);
        connect(mfc, &MFC::rangeChanged, this, &PlasmaController::handleSetMFCRangeCommand);
    }
    connect(&m_tuner, &Tuner::defaultRecipeChanged, this, &PlasmaController::handleSetTunerDefaultRecipeCommand);
    connect(&m_tuner, &Tuner::recipePositionChanged, this, &PlasmaController::handleSetTunerRecipePositionCommand);
    connect(&m_tuner, &Tuner::autoTuneChanged, this, &PlasmaController::handleSetTunerAutoTuneCommand);

    setupScanStateMachine();
}

PlasmaController::~PlasmaController()
{
    // Clean up the MFC objects
    for (MFC* mfc : m_mfcs) {
        delete mfc;
    }

    // cleanup shared serial resource
    if (m_pSerialInterface != nullptr) {
        if (m_pSerialInterface->isOpen()) {
            m_pSerialInterface->close();
        }
        delete m_pSerialInterface;
        m_pSerialInterface = nullptr;
    }

    // cleanup state machine states
    delete m_pScanStartupState;
    delete m_pParkZSubState;
    delete m_pGoXYStartSubState;
    delete m_pIdleSubState;
    delete m_pGoZScanPositionSubState;
    delete m_pScanColSubState;
    delete m_pScanRecycleState;
    delete m_pScanState;
    delete m_pScanSuperState;
    delete m_pScanShutdownState;
    delete m_pScanIdleState;
}

void PlasmaController::setupScanStateMachine()
{
    m_pScanSuperState = new QState();
    m_pScanStartupState = new QState(m_pScanSuperState);
    m_pScanState = new QState(m_pScanSuperState);
    m_pScanRecycleState = new QState(m_pScanSuperState);
    m_pScanShutdownState = new QState();
    m_pScanIdleState = new QState();
    // scan sub states
    m_pParkZSubState = new QState(m_pScanState);
    m_pGoXYStartSubState = new QState(m_pScanState);
    m_pIdleSubState = new QState(m_pScanState);
    m_pGoZScanPositionSubState = new QState(m_pScanState);
    m_pScanColSubState = new QState(m_pScanState);

    // set super state machine initial state and add it to the state machine
    m_pScanSuperState->setInitialState(m_pScanStartupState);
    m_scanStateMachine.addState(m_pScanSuperState);

    // set scan state machine initial state and add it to the state machine
    m_pScanState->setInitialState(m_pParkZSubState);
    m_scanStateMachine.addState(m_pScanState);

    // add the rest of the states
    m_scanStateMachine.addState(m_pScanShutdownState);
    m_scanStateMachine.addState(m_pScanIdleState);

    // add transitions to and from the super state
    m_pScanSuperState->addTransition(this, SIGNAL(SSM_TransitionIdle()), m_pScanIdleState);
    m_pScanSuperState->addTransition(this, SIGNAL(SSM_TransitionShutdown()), m_pScanShutdownState);

    // add transition to and from the scan state
    m_pScanState->addTransition(this, SIGNAL(SSM_TransitionRecycle()), m_pScanRecycleState);
    m_pScanRecycleState->addTransition(this, SIGNAL(SSM_TransitionScan()), m_pScanState);
    m_pScanStartupState->addTransition(this, SIGNAL(SSM_TransitionScan()), m_pScanState);

    m_pParkZSubState = new QState(m_pScanState);
    m_pGoXYStartSubState = new QState(m_pScanState);
    m_pIdleSubState = new QState(m_pScanState);
    m_pGoZScanPositionSubState = new QState(m_pScanState);
    m_pScanColSubState = new QState(m_pScanState);

    // add the rest of the transitions
    m_pParkZSubState->addTransition(this, SIGNAL(SSM_TransitionGoXYSubstate()), m_pGoXYStartSubState);
    m_pParkZSubState->addTransition(this, SIGNAL(SSM_TransitionIdleSubstate()), m_pIdleSubState);
    m_pGoXYStartSubState ->addTransition(this, SIGNAL(SSM_TransitionGoZPositionSubstate()), m_pGoZScanPositionSubState);
    m_pGoZScanPositionSubState->addTransition(this, SIGNAL(SSM_TransitionColSubstate()), m_pScanColSubState);
    m_pScanColSubState->addTransition(this, SIGNAL(SSM_TransitionIdleSubstate()), m_pIdleSubState);

    // set initial state
    m_scanStateMachine.setInitialState(m_pScanIdleState);

    // start the state machine
    m_scanStateMachine.start();
}

bool PlasmaController::open(const SettingsDialog& settings)
{
    const SettingsDialog::Settings p = settings.settings();
    m_pSerialInterface->setPortName(p.name);
    m_pSerialInterface->setBaudRate(p.baudRate);
    m_pSerialInterface->setDataBits(p.dataBits);
    m_pSerialInterface->setParity(p.parity);
    m_pSerialInterface->setStopBits(p.stopBits);
    m_pSerialInterface->setFlowControl(p.flowControl);
    if (!m_pSerialInterface->open(QIODevice::ReadWrite)) {
        // Failed to open the serial port
        return false;
    }

    // share the serial interface with the axes controller
    m_stageCTL.setSerialInterface(m_pSerialInterface);

    emit mainPortOpened();

    return true;
}


void PlasmaController::close()
{
    m_pSerialInterface->close();
}


bool PlasmaController::sendCommand(const QString& command)
{
    return m_pSerialInterface->sendCommand(command);
}

QString PlasmaController::readResponse()
{
    return m_pSerialInterface->readResponse();
}

void PlasmaController::setCommandMap(const QMap<QString, QPair<QString, QString>>& map)
{
    m_commandMap.setCommandMap(map);
}
QString PlasmaController::findCommandValue(QString command) const
{
    return m_commandMap.findCommandValue(command);
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

void PlasmaController::RunScanAxesSM()
{

}

MFC* PlasmaController::findMFCByNumber(int mfcNumber)
{
    for (MFC* mfc : m_mfcs) {
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
    return m_pSerialInterface->errorString();
}

bool PlasmaController::isOpen()
{
    if (m_pSerialInterface != nullptr) {
        return m_pSerialInterface->isOpen();
    }
    return false;
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
    m_ledStatus.setStatusBits(bits);
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
    m_tuner.setActualPosition(tunerPosition);

    // Extract and update RF power forward and reflected
    double rfForward = subsystemData[2].toDouble();
    double rfReflected = subsystemData[3].toDouble();
    m_pwr.setForwardWatts(rfForward);
    m_pwr.setReflectedWatts(rfReflected);

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
    m_plasmaHead.setTemperature(temperature);
}

bool PlasmaController::getExecuteRecipe() const
{
    return m_executeRecipe;
}

void PlasmaController::setExecuteRecipe(bool value)
{
    m_executeRecipe = value;
}

void PlasmaController::CTLStartup()
{
    howManyMFCs();
    getBatchIDLogging();
    getRecipeMBPosition();
    getRecipeRFPosition();
    getRecipeMFC4Flow();
    getRecipeMFC3Flow();
    getRecipeMFC2Flow();
    getRecipeMFC1Flow();
    getMFC4Range();
    getMFC3Range();
    getMFC2Range();
    getMFC1Range();
    getMaxRFPowerForward();
    //getAutoMan();
    //getTemp();
    //turnOffExecRecipe();

//    setCTLStateMachinesIdle();
}

void PlasmaController::howManyMFCs()
{
    sendCommand("$2A002%");
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7,1); //GET Number of MFCs (1-4) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
        bool ok = false;
        int numMFCs = StrVar.toInt(&ok);
        if (ok) {
            m_numMFCs = numMFCs;
            Logger::logInfo("Number of MFC's: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not set # of MFCs, last requestData: " + getLastCommand());
}

void PlasmaController::getBatchIDLogging() {
    sendCommand("$2A011%"); //GET BatchIDLogging $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, 1);
        bool ok = false;
        int batchLogging = StrVar.toInt(&ok);
        if (ok) {
            m_batchLogging = batchLogging;
            Logger::logInfo("Batch Logging On/Off: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve Batch Logging, last requestData sent: " + getLastCommand());
}

void PlasmaController::getRecipeMBPosition() {
    sendCommand("$2A606%"); //GET RECIPE MB Start Position () $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, 7);
        bool ok = false;
        double loadedSP = StrVar.toDouble(&ok);
        if (ok) {   
            m_readyToLoad = true;
            m_MBtunerRecipeSP = loadedSP;
            // update UI
            emit setRecipeMBtuner(StrVar);
            Logger::logInfo("Loaded MB Setpoint: " + StrVar + " %");
        }
    }
    else
        Logger::logCritical("Could Not retrieve MB tuner setpoint, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getRecipeRFPosition() {
    sendCommand("$2A605%"); //GET RECIPE RF PWR Setpoint (Watts) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, 4);
        bool ok = false;
        double rfLoadedSP = StrVar.toDouble(&ok);
        if (ok) {
            m_readyToLoad = true;
            m_RFtunerRecipeSP = rfLoadedSP;
            // update UI
            emit setRecipeRFpower(StrVar);
            Logger::logInfo("Loaded RF Setpoint: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve RF setpoint, last requestData sent: " + getLastCommand());
}

void PlasmaController::getRecipeMFC4Flow() {
    sendCommand("$2A604%"); //GET RECIPE MFC4 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRecipeFlow = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[3]->setRecipeFlow(mfcRecipeFlow);
            // update the UI
            emit MFC4RecipeFlow(QString::number(mfcRecipeFlow));
            Logger::logInfo("Loaded MFC 4 Flow Rate: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 4 setpoint, last requestData sent: " + getLastCommand());
}

void PlasmaController::getRecipeMFC3Flow() {
    sendCommand("$2A603%"); //GET RECIPE MFC3 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRecipeFlow = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[2]->setRecipeFlow(mfcRecipeFlow);
            // update the UI
            emit MFC3RecipeFlow(QString::number(mfcRecipeFlow));
            Logger::logInfo("Loaded MFC 3 Flow Rate: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 3 setpoint, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getRecipeMFC2Flow() {
    sendCommand("$2A602%"); //GET RECIPE MFC2 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRecipeFlow = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[1]->setRecipeFlow(mfcRecipeFlow);
            // update the UI
            emit MFC2RecipeFlow(QString::number(mfcRecipeFlow));
            Logger::logInfo("Loaded MFC 2 Flow Rate: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 2 setpoint, last requestData sent: " + getLastCommand() );
}
void PlasmaController::getRecipeMFC1Flow() {
    sendCommand("$2A601%"); //GET RECIPE MFC1 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRecipeFlow = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[0]->setRecipeFlow(mfcRecipeFlow);
            // update the UI
            emit MFC1RecipeFlow(QString::number(mfcRecipeFlow));
            Logger::logInfo("Loaded MFC 1 Flow Rate: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 1 setpoint, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getMFC4Range() {
    sendCommand("$8504%"); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    QString response = readResponse();
    if (response.length() > 6) {
        QString StrVar = response.mid(5, (response.length() - 6));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[3]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 4 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 4 range, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getMFC3Range() {
    sendCommand("$8503%"); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    QString response = readResponse();
    if (response.length() > 6) {
        QString StrVar = response.mid(5, (response.length() - 6));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[2]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 3 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 3 range, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getMFC2Range() {
    sendCommand("$8502%"); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    QString response = readResponse();
    if (response.length() > 6) {
        QString StrVar = response.mid(5, (response.length() - 6));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[1]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 2 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 2 range, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getMFC1Range() {
    sendCommand("$8501%"); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    QString response = readResponse();
    if (response.length() > 6) {
        QString StrVar = response.mid(5, (response.length() - 6));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[0]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 1 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + getLastCommand() );
}


void PlasmaController::getMaxRFPowerForward() {
    sendCommand("$2A705%"); //Get Max RF power forward  $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, 3);
        bool ok = false;
        int maxRFPowerForward = StrVar.toInt(&ok);
        if (ok) {
            m_MaxRFPowerForward = maxRFPowerForward;
            Logger::logInfo("Loaded Max RF Forward: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + getLastCommand());
}


void PlasmaController::getAutoMan() {
    sendCommand("$89%"); //GET_AUTO_MAN   $89%; resp [!890p#] p=1 AutoMode, p=0 ManualMode
    QString response = readResponse();
    if (response.length() > 3) {
        QString StrVar = response.mid(3, 2);
        bool ok = false;
        int autoTune = StrVar.toInt(&ok);
        if (ok) {
            m_autoTune = autoTune;
            Logger::logInfo("Loaded Tuner Auto Setting: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve Auto/Manual setting, last requestData sent: " + getLastCommand());
}

void PlasmaController::getTemp() {
    sendCommand("$8C%");
    QString response = readResponse();
    if (response.length() > 3) {
        QString StrVar = response.mid(3, 4);
        bool ok = false;
        double headTemp = StrVar.toDouble(&ok);
        if (ok) {
            m_headTemp = headTemp;
            // update UI
            emit plasmaHeadTemp(m_headTemp);
            Logger::logInfo("Loaded current temperature: " + StrVar);
        }
        else
            Logger::logCritical("Could Not retrieve temperature, last requestData sent: " + getLastCommand() );
    }
}

void PlasmaController::turnOffExecRecipe() {
    sendCommand("$8700%"); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
    readResponse();
    //ui->plsmaBtn->setText("START PLASMA"); // TODO
    Logger::logInfo("Execute Recipe : Disabled");
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
