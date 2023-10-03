#include "plasmacontroller.h"
#include "logger.h"

PlasmaController::PlasmaController(QWidget* parent)
  : QObject(parent),
    m_plasmaHead(),
    m_pwr(),
    m_tuner(),
    m_mfcs({ new MFC(1), new MFC(2), new MFC(3), new MFC(4) }),
    m_commandMap(),
    m_stageCTL(),
    m_config(),
    m_pRecipe(new PlasmaRecipe()),
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

    // setup state machines
    setupCollisionStateMachine();
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

    delete m_pRecipe;

    // cleanup scan state machine states
    delete m_pScanStartupState;
    delete m_pParkZSubState;
    delete m_pGoXYStartSubState;
    delete m_pGoZScanPositionSubState;
    delete m_pScanColSubState;
    delete m_pScanRecycleState;
    delete m_pScanShutdownState;
    delete m_pScanIdleState;
    delete m_pScanSuperState;

    // collision state machine
    delete m_pCPgetZUpstate;
    delete m_pCPStartupState;
    delete m_pCPScanYState;
    delete m_pCPGetZDownState;
    delete m_pCPIdleState;
    delete m_pCPShutdownState;
}

void PlasmaController::setupScanStateMachine()
{
    m_pScanSuperState = new QState();
    m_pScanStartupState = new QState(m_pScanSuperState);
    m_pScanRecycleState = new QState(m_pScanSuperState);
    m_pParkZSubState = new QState(m_pScanSuperState);
    m_pGoXYStartSubState = new QState(m_pScanSuperState);
    m_pGoZScanPositionSubState = new QState(m_pScanSuperState);
    m_pScanColSubState = new QState(m_pScanSuperState);
    m_pScanShutdownState = new QState();
    m_pScanIdleState = new QState();

    // set super state add it to the state machine
    m_scanStateMachine.addState(m_pScanSuperState);

    // add the rest of the states
    m_scanStateMachine.addState(m_pScanShutdownState);
    m_scanStateMachine.addState(m_pScanIdleState);
    m_scanStateMachine.setInitialState(m_pScanIdleState);

    // add transitions to and from the super state and idle and shutdown
    m_pScanSuperState->addTransition(this, SIGNAL(SSM_TransitionIdle()), m_pScanIdleState);
    m_pScanSuperState->addTransition(this, SIGNAL(SSM_TransitionShutdown()), m_pScanShutdownState);
    m_pScanIdleState->addTransition(this, SIGNAL(SSM_TransitionStartup()), m_pScanStartupState);
    m_pScanShutdownState->addTransition(this, SIGNAL(SSM_TransitionIdle()), m_pScanIdleState);

    // add the rest of the transitions
    m_pScanRecycleState->addTransition(this, SIGNAL(SSM_TransitionGoXYSubstate()), m_pGoXYStartSubState);
    m_pScanColSubState->addTransition(this, SIGNAL(SSM_TransitionParkZSubstate()), m_pParkZSubState);
    m_pScanStartupState->addTransition(this, SIGNAL(SSM_TransitionParkZSubstate()), m_pParkZSubState);
    m_pParkZSubState->addTransition(this, SIGNAL(SSM_TransitionGoXYSubstate()), m_pGoXYStartSubState);
    m_pParkZSubState->addTransition(this, SIGNAL(SSM_TransitionRecycle()), m_pScanRecycleState);
    m_pGoXYStartSubState ->addTransition(this, SIGNAL(SSM_TransitionGoZPositionSubstate()), m_pGoZScanPositionSubState);
    m_pGoZScanPositionSubState->addTransition(this, SIGNAL(SSM_TransitionScanColSubstate()), m_pScanColSubState);

    // start the state machine
    m_scanStateMachine.start();
}

void PlasmaController::setupCollisionStateMachine()
{
    m_pCPgetZUpstate = new QState();
    m_pCPStartupState = new QState();
    m_pCPScanYState = new QState();
    m_pCPGetZDownState = new QState();
    m_pCPIdleState = new QState();
    m_pCPShutdownState = new QState();

    // add states
    m_collisionStateMachine.addState(m_pCPgetZUpstate);
    m_collisionStateMachine.addState(m_pCPStartupState);
    m_collisionStateMachine.addState(m_pCPScanYState);
    m_collisionStateMachine.addState(m_pCPGetZDownState);
    m_collisionStateMachine.addState(m_pCPIdleState);
    m_collisionStateMachine.addState(m_pCPShutdownState);
    m_collisionStateMachine.setInitialState(m_pCPIdleState);

    // add transitions
    m_pCPIdleState->addTransition(this, SIGNAL(CSM_TransitionStartup()), m_pCPStartupState);
    m_pCPShutdownState->addTransition(this, SIGNAL(CSM_TransitionShutdown()), m_pCPIdleState);
    m_pCPStartupState->addTransition(this, SIGNAL(CSM_TransitionGetZUp()), m_pCPgetZUpstate);
    m_pCPgetZUpstate->addTransition(this, SIGNAL(CSM_TransitionScanY()), m_pCPScanYState);
    m_pCPScanYState->addTransition(this, SIGNAL(CSM_TransitionGetZDown()), m_pCPGetZDownState);
    m_pCPGetZDownState ->addTransition(this, SIGNAL(SSM_TransitionGoZPositionSubstate()), m_pCPShutdownState);

    m_collisionStateMachine.start();
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
    if (m_scanStateMachine.configuration().contains(m_pScanStartupState)) { // in Startup state

        emit SSM_Started();

        // load scan variables
        // translate from Displayed PH coords to moveable Base coords
        double minXPerPH = m_stageCTL.TranslateCoordXPH2Base(m_pRecipe->getXmin());
        double maxXPerPH = m_stageCTL.TranslateCoordXPH2Base(m_pRecipe->getXmax());

        if (maxXPerPH > minXPerPH) { // because coord systems can be flipped
            m_scanMaxXPos = maxXPerPH;
            m_scanMinXPos = minXPerPH;
        }
        else {
            m_scanMaxXPos = minXPerPH;
            m_scanMinXPos = maxXPerPH;
        }

        double minYPerPH = m_stageCTL.TranslateCoordYPH2Base(m_pRecipe->getYmin());
        double maxYPerPH = m_stageCTL.TranslateCoordYPH2Base(m_pRecipe->getYmax());

        if (maxYPerPH > minYPerPH) { // because coord systems can be flipped
            m_scanMaxYPos = maxYPerPH;
            m_scanMinYPos = minYPerPH;
        }
        else {
            m_scanMaxYPos = minYPerPH;
            m_scanMinYPos = maxYPerPH;
        }

        m_scanZParkPos = m_stageCTL.getZPinsBuriedPos();
        m_scanZScanPos = m_stageCTL.getZp2BaseDbl() - m_pRecipe->getThickness() - m_pRecipe->getGap();

        // get the scan row info
        m_scanRowXWidth = m_PHSlitWidth = m_pRecipe->getOverlap();
        double xLengthRemaining = m_scanMaxXPos - m_scanMinXPos;
        while (xLengthRemaining >= 0) {
            m_numXRows += 1;
            xLengthRemaining = xLengthRemaining - m_scanRowXWidth;
        }

        m_currentXRow = 1;
        m_numCycles = m_pRecipe->getCyclesInt();
        m_currentCycle = 1;

        if (m_numXRows == 1) { // have small substrate case, center the head over the center of the Box X
            m_startXPosition = (m_scanMaxXPos + m_scanMinXPos) / 2;
        }
        else { // multiple passes, so bias first pass to maximum edge
            m_startXPosition = m_scanMaxXPos - (m_scanRowXWidth / 2);// start position offset to center of slit
        }

        m_startYPosition;
        double m_scanYSpeed;
        double m_scanEndYPosition;

        // Y scan range from start to finish positions
        m_startYPosition = m_scanMaxYPos + m_PHSlitWidth;
        m_scanEndYPosition = m_scanMinYPos - m_PHSlitWidth;
        m_scanYSpeed = m_pRecipe->getSpeed();

        Logger::logInfo("-------------Scan Start-Up--------------");
        Logger::logInfo("Display MIN:(" + QString::number(m_scanMinXPos) + " , " + QString::number(m_scanMinYPos) + ") MAX:(" + QString::number(m_scanMaxXPos) + " , " + QString::number(m_scanMaxYPos) + ")");
        Logger::logInfo("Num Rows: " + QString::number(m_numXRows) + " Row Width: " + QString::number(m_scanRowXWidth, 'f', 2));
        Logger::logInfo("FirstX: " + QString::number(m_startXPosition, 'f', 2) + " StartY: " + QString::number(m_startYPosition, 'f', 2) + " EndY: " + QString::number(m_scanEndYPosition, 'f', 2));
        Logger::logInfo("Scan Speed: " + QString::number(m_scanYSpeed, 'f', 2) + " Cycles: " + QString::number(m_numCycles));

        if (m_collisionDetected == true && m_collisionPassed != true) { // if we have a laser, we need to perform collision test, once completed we can move into regualar scanning
            emit SSM_TransitionIdle(); // scan state machine to idle
            emit CSM_TransitionStartup(); // collision state machine to startup
        }
        else {
            emit SSM_TransitionParkZSubstate(); // scan state machine to park z
            emit SSM_StatusUpdate("Scanning"); // update ui
        }

        emit SSM_TransitionParkZSubstate();

        Logger::logInfo("In Scan Startup State");

    }
    else if (m_scanStateMachine.configuration().contains(m_pParkZSubState)) { // in parkz substate

        emit SSM_TransitionGoXYSubstate();

        Logger::logInfo("In Park Z Substate");

    }
    else if (m_scanStateMachine.configuration().contains(m_pGoXYStartSubState)) { // in goXY start substate

        emit SSM_TransitionGoZPositionSubstate();

        Logger::logInfo("In Go XY Substate");

    }
    else if (m_scanStateMachine.configuration().contains(m_pGoZScanPositionSubState)) { // in goZScanPosition substate

        emit SSM_TransitionScanColSubstate();

        Logger::logInfo("In Go Z Scan  Substate");

    }
    else if (m_scanStateMachine.configuration().contains(m_pScanColSubState)) { // in ScanCol substate

        emit SSM_TransitionParkZSubstate();

        Logger::logInfo("In Scan Col Substate");

    }
    else if (m_scanStateMachine.configuration().contains(m_pScanRecycleState)) { // in ScanRecycle substate


        Logger::logInfo("In Recycle Substate");

    }
    else if (m_scanStateMachine.configuration().contains(m_pScanShutdownState)) { // in Scan Shutdown state

        Logger::logInfo("In Shutdown state");

    }
    else if (m_scanStateMachine.configuration().contains(m_pScanIdleState)) { // in idle state

        Logger::logInfo("In Idle state");

    }

}

void PlasmaController::RunCollisionSM()
{
    if (m_collisionStateMachine.configuration().contains(m_pCPStartupState)) { // in parkz substate

        LaserSenseOn();
        Logger::logInfo("-------------Collision Pass Start-Up--------------");

        emit CSM_StatusUpdate("Collision Test");

        if (m_bPlasmaActive) {
            m_bRunRecipe = true;
        }

        emit CSM_TransitionGetZUp();

    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPgetZUpstate)) { // in goXY start substate
        if (m_stageCTL.nextStateReady()) {
            QString command = "$B402" + m_stageCTL.getZMaxSpeedQStr() + "%";
            sendCommand(command); //SET_SPEED  $B40xss.ss%; resp [!B40xss.ss#] 0x = axis number, ss.ss = mm/sec (float)
            QString response = readResponse();

            command = "$B602" + QString::number(m_scanZScanPos, 'f', 2) + "%";
            sendCommand(command); //ABS_MOVE $B60xaa.aa%; resp [!B60xaa.aa#] 0x = axis num, aa.aa = destination in mm (float)
            response = readResponse();

            QString LogStr = "Move Z at: " + m_stageCTL.getZMaxSpeedQStr() + "/sec ";
            Logger::logInfo(LogStr + "to: " + QString::number(m_scanZScanPos, 'f', 2));

            emit CSM_TransitionScanY();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPScanYState)) { // in goZScanPosition substate
        if (m_stageCTL.nextStateReady()) {
            QString command = "$B40110%";
            sendCommand(command); //SET_SPEED  $B40xss.ss%; resp [!B40xss.ss#] 0x = axis number, ss.ss = mm/sec (float)
            QString response = readResponse();

            command = "$B601" + QString::number(m_scanEndYPosition, 'f', 2) + "%";
            sendCommand(command); //ABS_MOVE $B60xaa.aa%; resp [!B60xaa.aa#] 0x = axis num, aa.aa = destination in mm (float)
            response = readResponse();

            QString LogStr = "Move Y at 10mm/sec ";
            Logger::logInfo(LogStr + "to: " + QString::number(m_scanEndYPosition, 'f', 2));

            emit CSM_TransitionGetZDown();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPGetZDownState)) { // in ScanCol substate
        if (m_stageCTL.nextStateReady()) {
            QString command = "$B402" + m_stageCTL.getZMaxSpeedQStr() + "%";
            sendCommand(command); //SET_SPEED  $B40xss.ss%; resp [!B40xss.ss#] 0x = axis number, ss.ss = mm/sec (float)
            readResponse();

            command = "$B602" + m_stageCTL.getZMaxSpeedQStr() + "%";
            sendCommand(command);  //ABS_MOVE $B60xaa.aa%; resp [!B60xaa.aa#] 0x = axis num, aa.aa = destination in mm (float)
            readResponse();

            QString LogStr = "Move Z Speed: " + m_stageCTL.getZMaxSpeedQStr() + " /sec ";
            Logger::logInfo(LogStr + "to " + m_stageCTL.getZMaxSpeedQStr());

            emit CSM_TransitionShutdown();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPShutdownState)) { // in Scan Shutdown state

        if (m_stageCTL.nextStateReady()) {

            emit CSM_StatusUpdate("Scanning");

            LaserSenseOff();
            m_collisionPassed = true;
            // Go here to scan
            if (m_plannedAutoStart == true) {
                m_bRunRecipe = true; //Turn plasma on
                m_plannedAutoStart = false;
            }
            else {
                emit SSM_TransitionStartup();
            }
        }

        emit CSM_TransitionIdle();

    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPIdleState)) { // in idle state
        // noop
    }

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
    QString command = "$30%";
    sendCommand(command);
    QString response = readResponse();
    int numMFCs = parseResponseForNumberOfMFCs(response);

    return numMFCs;
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
    sendCommand(command);
}

void PlasmaController::handleSetTunerDefaultRecipeCommand(const double defaultPosition)
{
    QString command = "$2A606" + QString::number(defaultPosition) + "%";
    sendCommand(command);
}

void PlasmaController::handleSetTunerAutoTuneCommand(const bool value)
{
    QString command = "$860" + QString::number(value) + "%";
    sendCommand(command);
}

void PlasmaController::handleSetPWRDefaultRecipeCommand(const double defaultWatts)
{
    QString command = "$2A605" + QString::number(defaultWatts) + "%";
    sendCommand(command);
}

void PlasmaController::handleSetPWRRecipeWattsCommand(const double recipeWatts)
{
    QString command = "$42" + QString::number(recipeWatts) + "%";
    sendCommand(command);
}

void PlasmaController::handleSetPWRMaxWattsCommand(const double maxWatts)
{
    QString command = "$2A705" + QString::number(maxWatts) + "%";
    sendCommand(command);
}

void PlasmaController::getCTLStatusCommand()
{
    QString command = "$91%";
    sendCommand(command);
    QString response = readResponse();
    parseResponseForCTLStatus(response);
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
    getAutoMan();
    getTemp();
    //turnOffExecRecipe();
    getPHSlitLength();
    getPHSlitWidth();

//    setCTLStateMachinesIdle();
}

void PlasmaController::LaserSenseOn()
{
    sendCommand("$BA01%"); // WATCH_SUBST_SENSE $BA0p%; resp[!BA0p#] p = 0, 1 turn watch (OFF, ON)
    readResponse();
    Logger::logInfo("Laser Sensor Active");
}

void PlasmaController::LaserSenseOff()
{
    sendCommand("$BA00%"); //  'WATCH_SUBST_SENSE $BA0p%; resp[!BA0p#] p = 0, 1 turn watch (OFF, ON)
    readResponse();
    Logger::logInfo("Laser Sense Deactived");
}

void PlasmaController::PollForCollision()
{
    if (m_stageCTL.getXAxisError() == 8 || // TODO: replace with symbolic constants
        m_stageCTL.getYAxisError() == 8 ||
        m_stageCTL.getZAxisError() == 8) {

        // a collision has been detected
        m_collisionDetected = true;
    }
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

void PlasmaController::getPHSlitLength()
{
    sendCommand("$DA540%"); //GET Plasma Head Slit Length (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSlitLength = StrVar.toDouble(&ok);
        if (ok) {
            m_PHSlitLength = PHSlitLength;
            Logger::logInfo("Plasma Head Slit Length: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Plasma head slit length, last requestData: " + getLastCommand());
}
void PlasmaController::getPHSlitWidth()
{
    sendCommand("$DA541%"); //GET Plasma Head Slit Width (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSlitWidth = StrVar.toDouble(&ok);
        if (ok) {
            m_PHSlitWidth = PHSlitWidth;
            Logger::logInfo("Plasma Head Slit Width: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Plasma head slit width, last requestData: " + getLastCommand());
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
