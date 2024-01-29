#include "plasmacontroller.h"
#include "logger.h"
#include "UtilitiesAndConstants.h"
#include "QApplication"

// file scope
bool estopActiveLast = false;
bool plasmaActiveLast = false;
bool abortLast = false;
int ledStatusLast = 0;

PlasmaController::PlasmaController(QWidget* parent)
  : QObject(parent),
    m_mfcs({ new MFC(1), new MFC(2), new MFC(3), new MFC(4), new MFC(5), new MFC(6) }), // a max of 6 mfc's
    m_lightTower(),
    m_pSerialInterface(new SerialInterface()),
    m_config(),
    m_stageCTL(),
    m_waferDiameter(),
    m_pRecipe(new PlasmaRecipe()),
    m_plasmaHead(),
    m_tuner(),
    m_pwr(),
    m_abortMessages(),
    m_scanMinXPos(0.0),
    m_scanMaxXPos(0.0),
    m_scanMinYPos(0.0),
    m_scanMaxYPos(0.0),
    m_scanZParkPos(0.0),
    m_scanZScanPos(0.0),
    m_scanRowXWidth(0.0),
    m_numXRows(0),
    m_currentXRow(0),
    m_numCycles(0),
    m_currentCycle(0),
    m_startXPosition(0.0),
    m_startYPosition(0.0),
    m_scanYSpeed(0.0),
    m_scanEndYPosition(0.0),
    m_ledStatus(0),
    m_abort(false),
    m_estopActive(false),
    m_plasmaActive(false),
    m_processDoorAbort(false),
    m_runRecipeOn(false),
    m_collisionDetected(false),
    m_collisionPassed(false),
    m_hasCollision(false),
    m_runRecipe(false), //Turn plasma on  
    m_plannedAutoStart(false)
{
    // Add startup data gathering methods.
    for (MFC* mfc: m_mfcs) {
        connect(mfc, &MFC::recipeFlowChanged, this, &PlasmaController::handleSetMFCRecipeFlowCommand);
    }
    connect(&m_tuner, &Tuner::recipePositionChanged, this, &PlasmaController::handleSetTunerRecipePositionCommand);
    connect(&m_tuner, &Tuner::autoTuneChanged, this, &PlasmaController::handleSetTunerAutoTuneCommand);
    connect(&m_pwr, &PWR::recipeWattsChanged, this, &PlasmaController::handleSetPWRRecipeWattsCommand);
    connect(&m_stageCTL, &AxesController::xLimitsChanged, this, &PlasmaController::xLimitsChanged);
    connect(&m_stageCTL, &AxesController::yLimitsChanged, this, &PlasmaController::yLimitsChanged);

    // setup state machines
    setupCollisionStateMachine();
    setupScanStateMachine();
    setupDoorOpenSM();

    // share the serial interface with the axes controller
    m_stageCTL.setSerialInterface(m_pSerialInterface);
}

PlasmaController::~PlasmaController()
{
    // Clean up the MFC objects
    for (MFC* mfc : m_mfcs) {
        delete mfc;
    }

    delete m_pSerialInterface;
    m_pSerialInterface = nullptr;

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

    // door open state machine
    delete m_pDODoorOpenedNonProcessState;
    delete m_pDODoorsClosedState;
    delete m_pDOWaitInitializedState;
    delete m_pDOGoToLoadState;
    delete m_pDOIdleState;
}

//////////////////////////////////////////////////////////////////////////////////
// State machine and execution
//////////////////////////////////////////////////////////////////////////////////

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

    // this allows AxesController to set the state machine to idle
    connect(&m_stageCTL, &AxesController::ScanSM_TransitionIdle, this, &PlasmaController::scanningStateMachineToIdle);

    // add transitions to and from the super state and idle and shutdown
    m_pScanSuperState->addTransition(this, &PlasmaController::SSM_TransitionIdle, m_pScanIdleState);
    m_pScanSuperState->addTransition(this, &PlasmaController::SSM_TransitionShutdown, m_pScanShutdownState);
    m_pScanIdleState->addTransition(this, &PlasmaController::SSM_TransitionStartup, m_pScanStartupState);
    m_pScanShutdownState->addTransition(this, &PlasmaController::SSM_TransitionIdle, m_pScanIdleState);
    m_pScanShutdownState->addTransition(this, &PlasmaController::SSM_TransitionIdle, m_pScanIdleState);

    // add the rest of the transitions
    m_pScanRecycleState->addTransition(this, &PlasmaController::SSM_TransitionGoXYSubstate, m_pGoXYStartSubState);
    m_pScanColSubState->addTransition(this, &PlasmaController::SSM_TransitionParkZSubstate, m_pParkZSubState);
    m_pScanStartupState->addTransition(this, &PlasmaController::SSM_TransitionParkZSubstate, m_pParkZSubState);
    m_pParkZSubState->addTransition(this, &PlasmaController::SSM_TransitionGoXYSubstate, m_pGoXYStartSubState);
    m_pParkZSubState->addTransition(this, &PlasmaController::SSM_TransitionRecycle, m_pScanRecycleState);
    m_pGoXYStartSubState ->addTransition(this, &PlasmaController::SSM_TransitionGoZPositionSubstate, m_pGoZScanPositionSubState);
    m_pGoZScanPositionSubState->addTransition(this, &PlasmaController::SSM_TransitionScanColSubstate, m_pScanColSubState);

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
    m_pCPIdleState->addTransition(this, &PlasmaController::CSM_TransitionStartup, m_pCPStartupState);
    m_pCPShutdownState->addTransition(this, &PlasmaController::CSM_TransitionIdle, m_pCPIdleState);
    m_pCPStartupState->addTransition(this, &PlasmaController::CSM_TransitionGetZUp, m_pCPgetZUpstate);
    m_pCPgetZUpstate->addTransition(this, &PlasmaController::CSM_TransitionScanY, m_pCPScanYState);
    m_pCPScanYState->addTransition(this, &PlasmaController::CSM_TransitionGetZDown, m_pCPGetZDownState);
    m_pCPGetZDownState ->addTransition(this, &PlasmaController::CSM_TransitionShutdown, m_pCPShutdownState);

    m_collisionStateMachine.start();
}

void PlasmaController::setupDoorOpenSM()
{
    m_pDODoorOpenedNonProcessState = new QState();
    m_pDODoorsClosedState = new QState();
    m_pDOWaitInitializedState = new QState();
    m_pDOGoToLoadState = new QState();
    m_pDOIdleState = new QState();

    // add states
    m_doorOpenStateMachine.addState(m_pDODoorOpenedNonProcessState);
    m_doorOpenStateMachine.addState(m_pDODoorsClosedState);
    m_doorOpenStateMachine.addState(m_pDOWaitInitializedState);
    m_doorOpenStateMachine.addState(m_pDOGoToLoadState);
    m_doorOpenStateMachine.addState(m_pDOIdleState);

    m_doorOpenStateMachine.setInitialState(m_pDOIdleState);

    // add transitions
    m_pDOIdleState->addTransition(this, &PlasmaController::DOSM_TransitionDoorOpenedNonProcess, m_pDODoorOpenedNonProcessState);
    m_pDOWaitInitializedState->addTransition(this, &PlasmaController::DOSM_TransitionDoorOpenedNonProcess, m_pDODoorOpenedNonProcessState);
    m_pDOGoToLoadState->addTransition(this, &PlasmaController::DOSM_TransitionDoorOpenedNonProcess, m_pDODoorOpenedNonProcessState);
    m_pDODoorsClosedState->addTransition(this, &PlasmaController::DOSM_TransitionDoorOpenedNonProcess, m_pDODoorOpenedNonProcessState);
    m_pDOIdleState->addTransition(this, &PlasmaController::DOSM_TransitionClosed, m_pDODoorsClosedState);
    m_pDODoorOpenedNonProcessState->addTransition(this, &PlasmaController::DOSM_TransitionClosed, m_pDODoorsClosedState);
    m_pDODoorsClosedState->addTransition(this, &PlasmaController::DOSM_TransitionWaitInitialized, m_pDOWaitInitializedState);
    m_pDOWaitInitializedState->addTransition(this, &PlasmaController::DOSM_TransitionLoad, m_pDOGoToLoadState);
    m_pDOGoToLoadState->addTransition(this, &PlasmaController::DOSM_TransitionIdle, m_pDOIdleState);

    m_doorOpenStateMachine.start();
}

void PlasmaController::RunScanAxesSM()
{
    if (m_scanStateMachine.configuration().contains(m_pScanStartupState)) { // in Startup state

        emit SSM_Started();

        // load scan variables
        // recipe variables are in PH so translate to moveable base coords
        double minXBase = m_stageCTL.TranslateCoordXPH2Base(m_pRecipe->getXminPH());
        double maxXBase = m_stageCTL.TranslateCoordXPH2Base(m_pRecipe->getXmaxPH());

        if (maxXBase > minXBase) { // because coord systems can be flipped
            m_scanMaxXPos = maxXBase;
            m_scanMinXPos = minXBase;
        }
        else {
            m_scanMaxXPos = minXBase;
            m_scanMinXPos = maxXBase;
        }

        // load scan variables
        // recipe variables are in PH so translate to moveable base coords
        double minYBase = m_stageCTL.TranslateCoordYPH2Base(m_pRecipe->getYminPH());
        double maxYBase = m_stageCTL.TranslateCoordYPH2Base(m_pRecipe->getYmaxPH());

        if (maxYBase > minYBase) { // because coord systems can be flipped
            m_scanMaxYPos = maxYBase;
            m_scanMinYPos = minYBase;
        }
        else {
            m_scanMaxYPos = minYBase;
            m_scanMinYPos = maxYBase;
        }

        m_scanZParkPos = m_stageCTL.getZPinsBuriedPos();
        m_scanZScanPos = m_stageCTL.getZp2BaseDbl() - m_pRecipe->getThickness() - m_pRecipe->getGap();

        // get the scan row info
        m_scanRowXWidth = m_plasmaHead.getSlitLength() - m_pRecipe->getOverlap();
        double xLengthRemaining = m_scanMaxXPos - m_scanMinXPos;
        m_numXRows = 0;
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

        // Y scan range from start to finish positions
        m_startYPosition = m_scanMaxYPos + m_plasmaHead.getSlitWidth();
        m_scanEndYPosition = m_scanMinYPos - m_plasmaHead.getSlitWidth();
        m_scanYSpeed = m_pRecipe->getSpeed();

        Logger::logInfo("-------------Scan Start-Up--------------");
        Logger::logInfo("Display MIN:(" + m_pRecipe->getXminPHQStr() + " , " + m_pRecipe->getXmaxPHQStr() + ") MAX:(" +
                        m_pRecipe->getYminPHQStr() + " , " + m_pRecipe->getYmaxPHQStr() + ")");
        Logger::logInfo("Num Rows: " + QString::number(m_numXRows) + " Row Width: " + QString::number(m_scanRowXWidth, 'f', 2));
        Logger::logInfo("FirstX: " + QString::number(m_startXPosition, 'f', 2) + " StartY: " + QString::number(m_startYPosition, 'f', 2) + " EndY: " + QString::number(m_scanEndYPosition, 'f', 2));
        Logger::logInfo("Scan Speed: " + QString::number(m_scanYSpeed, 'f', 2) + " Cycles: " + QString::number(m_numCycles));

        if (m_hasCollision && !m_collisionPassed) { // if we have a laser, we need to perform collision test, once completed we can move into regualar scanning
            emit SSM_TransitionIdle(); // scan state machine to idle
            emit CSM_TransitionStartup(); // collision state machine to startup
        }
        else {
            emit SSM_TransitionParkZSubstate(); // scan state machine to park z
            emit SSM_StatusUpdate("Scanning", ""); // update ui
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pParkZSubState)) { // in parkz substate

        if (m_stageCTL.nextStateReady()) {
            if (m_currentXRow > m_numXRows) {
                emit SSM_TransitionRecycle();
                QString message = "End Cycle #" + QString::number(m_currentCycle) + " of " + QString::number(m_numCycles);
                Logger::logInfo(message);
                emit SSM_StatusUpdate("Scanning", message); // update ui
            }
            else {
                emit SSM_TransitionGoXYSubstate();
                QString message = "Parking Z";
                Logger::logInfo(message);
                emit SSM_StatusUpdate("Scanning", message); // update ui
            }
            // turn off Substrate N2 Purge (assume it's on)
            // m_stageCTL.toggleN2PurgeOff();

            // set max Z speed and move to parkZ
            m_stageCTL.move(ZAXIS_COMMAND_NUM, m_stageCTL.ZMaxSpeed(), m_scanZParkPos);
            // log the move
            QString logStr = "Move Z Speed: " + m_stageCTL.getZMaxSpeedQStr() + " /sec ";
            Logger::logInfo(logStr + "to " + QString::number(m_scanZParkPos, 'f', 2));
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pGoXYStartSubState)) { // in goXY start substate
        if (m_stageCTL.nextStateReady()) {
            QString message = "Scan Cycle #" + QString::number(m_currentCycle) + " of " + QString::number(m_numCycles) +
                              " Traversal #" + QString::number(m_currentXRow) + " of " + QString::number(m_numXRows);
            Logger::logInfo(message);

            emit SSM_StatusUpdate("Scanning", message); // update ui

            // log it
            QString logStr = "X Speed: " + m_stageCTL.getXAxisMaxSpeedQStr() + " /sec ";
            Logger::logInfo(logStr + "Y Speed: " + m_stageCTL.getYAxisMaxSpeedQStr() + "/sec");

            if (m_currentXRow > 1) {
                m_startXPosition = m_startXPosition- m_scanRowXWidth; // move over one
            }

            // move X to starting position
            m_stageCTL.move(XAXIS_COMMAND_NUM, m_stageCTL.XMaxSpeed(), m_startXPosition);
            // move Y to starting position
            m_stageCTL.move(YAXIS_COMMAND_NUM, m_stageCTL.YMaxSpeed(), m_startYPosition);

            // log it
            logStr = "X to: " + QString::number(m_startXPosition, 'f', 2);
            Logger::logInfo(logStr + " Y to: " + QString::number(m_startYPosition, 'f', 2));
            emit SSM_TransitionGoZPositionSubstate();
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pGoZScanPositionSubState)) { // in goZScanPosition substate
        if (m_stageCTL.nextStateReady()) {
            if (m_pRecipe->getPurge()) { // turn on the substrate N2 Purge
                m_stageCTL.toggleN2PurgeOn();
            }
            // set max Z speed and move to Z scan start
            m_stageCTL.move(ZAXIS_COMMAND_NUM, m_stageCTL.ZMaxSpeed(), m_scanZScanPos);
            // log it
            QString logStr = "Move Z at: " + m_stageCTL.getZMaxSpeedQStr() + " /sec ";
            Logger::logInfo(logStr + "to: " + QString::number(m_scanZScanPos, 'f', 2));
            emit SSM_TransitionScanColSubstate();
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pScanColSubState)) { // in ScanCol substate
        if (m_stageCTL.nextStateReady()) {
            // set Y scan speed and move to Y scan end position
            m_stageCTL.move(YAXIS_COMMAND_NUM, m_scanYSpeed, m_scanEndYPosition);
            // log it
            QString logStr = "Move Y at " + QString::number(m_scanYSpeed, 'f', 2) + " /sec ";
            Logger::logInfo(logStr + "to: " + QString::number(m_scanEndYPosition, 'f', 2));
            m_currentXRow += 1; // increment row counter
            emit SSM_TransitionParkZSubstate();
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pScanRecycleState)) { // in ScanRecycle substate
        if (m_currentCycle >= m_numCycles) { // do not recycle

            // Assume N2 is on, turn it off
            m_stageCTL.toggleN2PurgeOff();

            // Reset the collision flag if using a collision system
            if (m_collisionPassed == true) {
                m_collisionPassed = false; // reset the collision flag
            }
            
            bool done = true; // controls going to load and auto off
            
            if (m_pRecipe->getNumCascadeRecipes() > 1) { // there are cascade recipes in the list so we must be running multiple recipes

                // don't load the next recipe until the stage is out of the way
                if (m_stageCTL.getZPosition() <= m_scanZParkPos) {

                    emit SSM_TransitionIdle();
                    emit SSM_Done();

                    // increment the index tracker
                    m_pRecipe->incrementCascadeIndex();

                    int currentCascadeIndex = m_pRecipe->getCurentCascadeIndex();
                    int numCascadeRecipes = m_pRecipe->getNumCascadeRecipes();

                    if (currentCascadeIndex < numCascadeRecipes) {
                        // prevents going to load since we are running another recipe
                        done = false;
                    }

                    if (done) { // we are running a single recipe or we have run all cascade recipes so we are done
                        m_stageCTL.StartHome(); // Go to the Load position everytime you finish scanning
                        // Auto off will turn the recipe off and PLASMA.
                        if (m_pRecipe->getAutoScanBool()) {
                            Logger::logInfo("Plasma turned off (Auto-Off is active)");
                            sendCommand("$8700%");
                            readResponse();
                        }
                    }

                    emit loadCascadeRecipe(); // loads the next recipe as specified by the cascade index
                } 
            }
            else {

                emit SSM_TransitionIdle();
                emit SSM_Done();

                m_stageCTL.StartHome(); // Go to the Load position everytime you finish scanning
                // Auto off will turn the recipe off and PLASMA.
                if (m_pRecipe->getAutoScanBool()) {
                    Logger::logInfo("Plasma turned off (Auto-Off is active)");
                    sendCommand("$8700%");
                    readResponse();
                }
            }
        }
        else { // recycle the scan
            m_currentCycle += 1;
            m_currentXRow = 1;

            if (m_numXRows == 1) { // 'have small substrate case, center the head over the center of the Box X
                m_startXPosition = (m_scanMaxXPos + m_scanMinXPos) / 2;
            }
            else { // multiple passes, so bias first pass to maximum edge
                m_startXPosition = m_scanMaxXPos - (m_scanRowXWidth / 2); // start position offset to center of slit
            }

            Logger::logInfo("-------------Scan Recycle Start-Up--------------This Cycle: " + QString::number(m_currentCycle));
            Logger::logInfo("Display MIN:(" + m_pRecipe->getXminPHQStr() + " , " + m_pRecipe->getYminPHQStr() + ") MAX:(" + m_pRecipe->getXmaxPHQStr() + " , " + m_pRecipe->getYmaxPHQStr() + ")");
            Logger::logInfo("Num Rows: " + QString::number(m_numXRows) + " Row Width: " + QString::number(m_scanRowXWidth, 'f', 2));
            Logger::logInfo("FirstX: " + QString::number(m_startXPosition, 'f', 2) + " StartY: " + QString::number(m_startYPosition, 'f', 2) + " EndY: " + QString::number(m_scanEndYPosition, 'f', 2));
            Logger::logInfo("Scan Speed: " + QString::number(m_scanYSpeed, 'f', 2) + " Cycles: " + QString::number(m_numCycles));

            emit SSM_TransitionGoXYSubstate(); // reenter here because already Parked Z
        }
    }
    else if (m_scanStateMachine.configuration().contains(m_pScanShutdownState)) { // in Scan Shutdown state

        emit SSM_StatusUpdate("Scanning Stopped - Parking Z", ""); // update ui

        // stop any motors in motion
        m_stageCTL.stopAllMotors();
        // turn off Substrate N2 Purge (assume it's on)
        m_stageCTL.toggleN2PurgeOff();

        // set Z max speed and move to Z park position
        m_stageCTL.move(ZAXIS_COMMAND_NUM, m_stageCTL.ZMaxSpeed(), m_scanZParkPos);

        // log it
        QString logStr = "Move Z at " + m_stageCTL.getZMaxSpeedQStr() + " /sec ";
        Logger::logInfo(logStr + "to: " + QString::number(m_scanZParkPos, 'f', 2));

        // done
        emit SSM_TransitionIdle();
        emit SSM_StatusUpdate("Scanning Manually Stopped", ""); // update ui
        emit SSM_Done();

        m_collisionPassed = false; // reset the collision flag
    }
    else if (m_scanStateMachine.configuration().contains(m_pScanIdleState)) { // in idle state

    }
}

void PlasmaController::RunCollisionSM()
{
    if (m_collisionStateMachine.configuration().contains(m_pCPStartupState)) {

        LaserSenseOn();
        Logger::logInfo("-------------Collision Pass Start-Up--------------");

        emit CSM_StatusUpdate("Collision Test", "");

        if (m_plasmaActive) {
            m_runRecipe = true;
        }

        emit CSM_TransitionGetZUp();

    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPgetZUpstate)) {
        if (m_stageCTL.nextStateReady()) {

            // set max Z speed and move to Z scan position
            m_stageCTL.move(ZAXIS_COMMAND_NUM, m_stageCTL.ZMaxSpeed(), m_scanZScanPos);

            QString LogStr = "Move Z at: " + m_stageCTL.getZMaxSpeedQStr() + "/sec ";
            Logger::logInfo(LogStr + "to: " + QString::number(m_scanZScanPos, 'f', 2));

            emit CSM_TransitionScanY();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPScanYState)) {
        if (m_stageCTL.nextStateReady()) {
            // set Y speed = 10mm/sec and move to Y scan end position
            m_stageCTL.move(YAXIS_COMMAND_NUM, 10.0, m_scanEndYPosition);

            QString LogStr = "Move Y at 10mm/sec ";
            Logger::logInfo(LogStr + "to: " + QString::number(m_scanEndYPosition, 'f', 2));

            emit CSM_TransitionGetZDown();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPGetZDownState)) {
        if (m_stageCTL.nextStateReady()) {
            // set max Z speed and move to parkZ
            m_stageCTL.move(ZAXIS_COMMAND_NUM, m_stageCTL.ZMaxSpeed(), m_scanZParkPos);

            QString LogStr = "Move Z Speed: " + m_stageCTL.getZMaxSpeedQStr() + " /sec ";
            Logger::logInfo(LogStr + "to " + QString::number(m_scanZParkPos, 'f', 2));

            emit CSM_TransitionShutdown();
        }
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPShutdownState)) {

        if (m_stageCTL.nextStateReady()) {

            emit CSM_StatusUpdate("Scanning", "");

            LaserSenseOff();
            m_collisionPassed = true;
            // Go here to scan
            if (m_plannedAutoStart) {
                RunRecipe(true);
                m_plannedAutoStart = false;
            }
            else {
                emit SSM_TransitionStartup();
            }

            emit CSM_TransitionIdle();
        }  
    }
    else if (m_collisionStateMachine.configuration().contains(m_pCPIdleState)) { // in idle state
        // noop
    }
}

void PlasmaController::RunDoorOpenSM()
{
    // look for door open conditions
    if (m_processDoorAbort) {
        emit DOSM_TransitionClosed();
    }
    else if (m_stageCTL.getDoorOpen() && stateMachineActive()) {
        emit DOSM_TransitionDoorOpenedNonProcess();
    }

    if (m_doorOpenStateMachine.configuration().contains(m_pDODoorOpenedNonProcessState)) {
        QString abortMessage = "Door opened while stage was moving. Stage position has been lost, close the doors and click OK to initialize the stage and send stage to the Load position.";
        emit m_abortMessages.showAbortMessageBox(abortMessage, false);
        emit DOSM_TransitionClosed();
    }
    else if (m_doorOpenStateMachine.configuration().contains(m_pDODoorsClosedState)) {
        if (!m_stageCTL.getDoorOpen()) {
            Logger::logInfo("Stage position lost : doors opened");
            m_processDoorAbort = false;
            // state machines to idle
            m_stageCTL.setAxisStateMachinesIdle();
            // initialize
            m_stageCTL.StartInit();
            emit DOSM_TransitionWaitInitialized();
        }
    }
    else if (m_doorOpenStateMachine.configuration().contains(m_pDOWaitInitializedState)) {
        if (m_stageCTL.getAxesInitilizedStatus()) {
            emit DOSM_TransitionLoad();
        }
    }
    else if (m_doorOpenStateMachine.configuration().contains(m_pDOGoToLoadState)) {
        emit m_stageCTL.HSM_TransitionStartup(); // go to load
        emit DOSM_TransitionIdle();
    }
    else if (m_doorOpenStateMachine.configuration().contains(m_pDOIdleState)) { // in idle state

    }
}

bool PlasmaController::stateMachineActive()
{
    if (!m_stageCTL.axisStateMachineActive() &&
        m_scanStateMachine.configuration().contains(m_pScanIdleState) &&
        m_collisionStateMachine.configuration().contains(m_pCPIdleState)) {
            return false;
    }
    else {
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Serial Port
//////////////////////////////////////////////////////////////////////////////////

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

MFC* PlasmaController::findMFCByNumber(int mfcNumber)
{
    for (MFC* mfc : m_mfcs) {
        if (mfc->getMFCNumber() == mfcNumber) {
            return mfc;
        }
    }
    return nullptr;
}

void PlasmaController::runDiameter()
{
    double radius =  m_waferDiameter.getCurrentWaferDiameterSelection() / 2.0;

    double Xp2Base = m_stageCTL.getXPH2Base();
    double Yp2Base = m_stageCTL.getYPH2Base();

    double xBaseMin = (Xp2Base - radius);
    double xBaseMax = (Xp2Base + radius);
    double yBaseMin = (Yp2Base - radius);
    double yBaseMax = (Yp2Base + radius);

    // update the "actual" display after coord sys translation
    double dbVal = m_stageCTL.TranslateCoordXBase2PH(xBaseMin);
    m_pRecipe->setXminPH(dbVal);
    dbVal = m_stageCTL.TranslateCoordXBase2PH(xBaseMax);
    m_pRecipe->setXmaxPH(dbVal);
    dbVal = m_stageCTL.TranslateCoordYBase2PH(yBaseMin);
    m_pRecipe->setYminPH(dbVal);
    dbVal = m_stageCTL.TranslateCoordYBase2PH(yBaseMax);
    m_pRecipe->setYmaxPH(dbVal);

    // trigger a ui update
    emit scanBoxChanged();
}

void PlasmaController::setLEDLightIntensity(int percent)
{
    //$C9pp.p% resp[!C9pp.p#] Percent power to Illumination
    QString LEDCommand = "$C9" + QString::number(percent) + ".0%";
    sendCommand(LEDCommand);
    readResponse();
}

bool PlasmaController::has3Axis()
{
    bool returnVal = true;
    sendCommand("$22000D%");
    QString response = readResponse();
    //response = "$22000D0%";
    if (response.length() > 8) {
        QString StrVar = response.mid(7, 1);
        returnVal = QVariant(StrVar).toBool();
    }
    return returnVal;
}

void PlasmaController::handshakeOn(bool on)
{
    if (on) {
        sendCommand("$1E01%");
        readResponse();
    }
    else {
        sendCommand("$1E00%");
        readResponse();
    }
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

void PlasmaController::handleSetDefaultRecipeCommand()
{
    sendCommand("$44%"); // SET_DEF_RECIPE $44%; resp [!44#]
    readResponse(); // clear the response buffer
}

void PlasmaController::batchIDLoggingOn(bool state)
{
    if (state) {
        sendCommand("$28011;1%"); // $28xxx;vv..vv%, xxxx = any length index number, vv..vv = value; =>resp [!28xxxx;vv..vv#]
        readResponse();
        Logger::logInfo("Batch ID logging toggled ON");
    }
    else {
        sendCommand("$28011;0%"); // $28xxx;vv..vv%, xxxx = any length index number, vv..vv = value; =>resp [!28xxxx;vv..vv#]
        readResponse();
        Logger::logInfo("Batch ID logging toggled OFF");
    }
}

// MFC
void PlasmaController::handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow)
{
    QString mfcNum = "0" + QString::number(mfcNumber);
    QString mfcRecipeFlow = QStringLiteral("%1").arg(recipeFlow, 5, 'f', 2, QChar('0')); // number, 6=total field length, 'f'=dont use scientific notation, 2=number of decimal places, padding char
    QString command = "$41" + mfcNum + mfcRecipeFlow + "%";
    sendCommand(command);  // SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
    readResponse();
}

// TUNER
void PlasmaController::handleSetTunerRecipePositionCommand(const int recipePosition)
{   
    QString number = QString("%1").arg(recipePosition, 4, 10, QChar('0')); // number, field width, base, fill char
    QString command = "$43" + number + "%"; // SET_RCP_MS_POS  $43xxxx$ xxxx = Base10 MB Motor Pos; resp[!43xxxx#]
    sendCommand(command);
    readResponse();
}

void PlasmaController::handleSetTunerAutoTuneCommand(const bool value)
{
    QString command = "$860" + QString::number(value) + "%"; // SET_AUTO_MAN 0x86 //$860p% p=1 AutoMode, p=0 ManualMode
    sendCommand(command);
    readResponse();
}

void PlasmaController::handleSetPWRRecipeWattsCommand(const int recipeWatts)
{
    QString number = QString("%1").arg(recipeWatts, 4, 10, QChar('0')); // number, field width, base, fill char
    QString command = "$42" + number + "%"; // SET_RCP_RF_WATTS  $42xxxx% xxxx = Watts; resp[!42xxxx#]
    sendCommand(command);
    readResponse();
}

void PlasmaController::RunRecipe(const bool newRunState) // handles turning the recipe on/off
{
    QString response;
    if (newRunState) {
        sendCommand("$8701%"); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
        response = readResponse();
        Logger::logInfo("Execute Recipe : Enabled");
        emit recipeExecutionStateChanged(true);
    }
    else {
        sendCommand("$8700%"); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
        response = readResponse();
        Logger::logInfo("Execute Recipe : Disabled");
        emit recipeExecutionStateChanged(false);
    }
}

void PlasmaController::getCTLStatusCommand()
{
    QString command = "$91%";
    sendCommand(command);
    QString response = readResponse();
    parseResponseForCTLStatus(response);
}

void PlasmaController::handleAutoScan()
{
    if (m_plasmaActive && m_pRecipe->getAutoScanBool()) {
        if (m_stageCTL.getXAxisState() >= AXIS_IDLE && m_stageCTL.getYAxisState() >= AXIS_IDLE  && m_stageCTL.getZAxisState() >= AXIS_IDLE ) {
            StartScan();
        }
    }
}

void PlasmaController::xLimitsChanged(double xmin, double xmax)
{
    m_pRecipe->setXminPH(xmin);
    m_pRecipe->setXmaxPH(xmax);
}

void PlasmaController::yLimitsChanged(double ymin, double ymax)
{
    m_pRecipe->setYminPH(ymin);
    m_pRecipe->setYmaxPH(ymax);
}

void PlasmaController::plasmaStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_PLASMA_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_plasmaActive = isBitSet(m_ledStatus, bit);

        if (m_plasmaActive != plasmaActiveLast) {
            plasmaActiveLast = m_plasmaActive;

            if (m_plasmaActive)
                Logger::logInfo("Plasma : on");
            else
                Logger::logInfo("Plasma : off");

            emit plasmaStateChanged(m_plasmaActive);
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_PLASMA_STATUS_BIT));
    }
}

void PlasmaController::estopStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_ESTOP_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_estopActive = isBitSet(m_ledStatus, bit);

        if (m_estopActive != estopActiveLast) {
            estopActiveLast = m_estopActive;

            if (m_estopActive)
                Logger::logInfo("Estop : open");
            else
                Logger::logInfo("Estop : closed");
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_ESTOP_STATUS_BIT));
    }
}

void PlasmaController::abortStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_ABORT_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_abort = isBitSet(m_ledStatus, bit);

        if (m_abort != abortLast) {
            abortLast = m_abort;

            if (m_abort) {
                Logger::logInfo("Abort : active");

                sendCommand("$8B%"); // GETSET_ABORT_CODE  $8B%; resp [!8Bcccc#] cccc = Base10 Abort Code
                QString response = readResponse();

                parseAbortCode(response);
            }
            else
                Logger::logInfo("Abort : cleared");
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_ABORT_STATUS_BIT));
    }
}

void PlasmaController::parseAbortCode(QString code)
{
    if (code.length() > 7) {
        QString abortCodeStr = code.mid(3,4);

        bool ok = false;
        int abortCodeInt = abortCodeStr.toInt(&ok, 16);
        if (ok) {
            if (abortCodeInt > 0) {
                if (m_abortMessages.containsAbortMessage(abortCodeInt)) {
                    QString abortMessage = m_abortMessages.getAbortMessage(abortCodeInt);
                    // log the particular abort code/message
                    Logger::logCritical(abortMessage);
                    // the door abort code is treated differently so retain
                    if (abortCodeInt == AbortCodeMessages::AC_DOORS_OPEN)
                        m_processDoorAbort = true;
                    // immediately set the light tower
                    setLightTower();

                    if (abortCodeInt == AbortCodeMessages::AC_ESTOP) {
                        // show a message box
                        emit m_abortMessages.showAbortMessageBox(abortMessage, true);
                    }
                    else {
                        emit m_abortMessages.showAbortMessageBox(abortMessage, false);
                    }
                }
                else {
                    QString errorMessage = "Abort occurred, no corresponding abort code found.";
                    Logger::logCritical(errorMessage);
                    emit m_abortMessages.showAbortMessageBox(errorMessage, false);
                }
            }
        }
        else {
            QString errorMessage = "Abort occurred, no corresponding abort code found.";
            Logger::logCritical(errorMessage);
            emit m_abortMessages.showAbortMessageBox(errorMessage, false);
        }
    }
}

void PlasmaController::parseResponseForCTLStatus(const QString& response)
{    
    // Extract LED status
    QString ledStatusHex = response.mid(3, 4); // Assuming LLRR are at positions 3 to 6
    bool ok;
    m_ledStatus = ledStatusHex.toInt(&ok, 16);
    if (ok) {
        // update
        plasmaStatus();
        estopStatus();
        abortStatus();
    }

    // Split the response string into subsystem data
    QStringList subsystemData = response.mid(6, response.length() - 7).split(";");

    // Check if we have enough data to update the subsystems
    //if (subsystemData.size() != 12) {
    if (subsystemData.size() != 14) {
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
    setRecipeExecuting(execRecipe);

    // Extract and update MFC actual flow values
    for (int i = 1; i <= 6; i++) {
        MFC* mfc = findMFCByNumber(i);
        double mfcFlow = subsystemData[4 + i].toDouble();
        mfc->setActualFlow(mfcFlow);
    }

    // Extract and update plasmahead temperature
    double temperature = subsystemData[11].toDouble();
    m_plasmaHead.setTemperature(temperature);
}

void PlasmaController::setLightTower()
{
    if (m_processDoorAbort || m_stageCTL.getDoorOpen()) {
        m_lightTower.setState(LightTower::LT_ERROR);
    }
    else if ( (!m_stageCTL.getDoorOpen()) && (m_runRecipeOn) ) {
        m_lightTower.setState(LightTower::LT_ACTIVE);
    }
    else {
        if (m_lightTower.getState() != LightTower::LT_READY) {
            m_lightTower.setState(LightTower::LT_READY);
        }
    }

    switch(m_lightTower.getState()) {
    case LightTower::LT_ERROR: // Light tower turns red - error commands and dangrous conditions
        sendCommand("$CB01%"); //  $C    sendCommand("$8701%"); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
        readResponse();
        m_lightTower.setState(LightTower::LT_INACTIVE);
        break;
    case LightTower::LT_READY: // Light tower turns amber - ready to recieve/process commands
        sendCommand("$CB02%"); //  $CB0n% resp[!CB0n#] n = 0,1,2,3 (none, red, amber, green)
        readResponse();
        m_lightTower.setState(LightTower::LT_INACTIVE);
        break;
    case LightTower::LT_ACTIVE:
        sendCommand("$CB03%"); //  $CB0n% resp[!CB0n#] n = 0,1,2,3 (none, red, amber, green)
        readResponse();
        m_lightTower.setState(LightTower::LT_INACTIVE);
        break;
    case LightTower::LT_INACTIVE:
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Recipe
//////////////////////////////////////////////////////////////////////////////////
bool PlasmaController::getExecuteRecipe() const
{
    return m_runRecipeOn;
}

void PlasmaController::setRecipeExecuting(bool value)
{
    m_runRecipeOn = value;
}

void PlasmaController::setRecipe(QString filePath)
{
    m_pRecipe->fileReader.setFilePath(filePath);
    m_pRecipe->setRecipeFromFile();
    // process the keys that have CTL board interaction
    setMFCsFlowFromRecipe();
    setRFSetpointFromRecipe();
    setTunerSetpointFromRecipe();
    // as per cory Auto Tune should be set to on when loading any recipe
    m_tuner.setAutoTune(true);
}

void PlasmaController::setMFCsFlowFromRecipe()
{
    for (int i = 0; i < m_mfcs.size(); i++)
    {
        MFC* mfc = m_mfcs.at(i);
        QString mfcKey = RECIPE_MFC_KEY + QString::number(i + 1);

        if (m_pRecipe->getRecipeMap().contains(mfcKey)) {
            double flow = m_pRecipe->getRecipeMap().value(mfcKey).toDouble();
            mfc->setRecipeFlow(flow);
        }
        else  {
            // Handle the case when the MFC key is not found in the recipe map
            Logger::logWarning(RECIPE_MFC_KEY + QString::number(i+1) + " setpoint not found in recipe map.");
        }
    }
}

void PlasmaController::setRFSetpointFromRecipe()
{
    if (m_pRecipe->getRecipeMap().contains(RECIPE_PWR_KEY)) {
        int watts = m_pRecipe->getRecipeMap().value(RECIPE_PWR_KEY).toInt();
        m_pwr.setRecipeWatts(watts);
    }
    else {
        // Handle the case when "PWR" key is not found in the recipe map
        Logger::logWarning(QString(RECIPE_PWR_KEY) + " setpoint not found in recipe map.");
    }
}

void PlasmaController::setTunerSetpointFromRecipe()
{
    if (m_pRecipe->getRecipeMap().contains(RECIPE_TUNER_KEY)) {
        double position = m_pRecipe->getRecipeMap().value(RECIPE_TUNER_KEY).toDouble();
        m_tuner.setRecipePosition(position);
    }
    else {
        // Handle the case when "TUNER" key is not found in the recipe map
        Logger::logWarning(QString(RECIPE_TUNER_KEY) + " setpoint not found in recipe map.");
    }
}

void PlasmaController::CTLStartup(bool has3AxisBoard)
{
    getFirmwareVersion();
    howManyMFCs();
    getBatchIDLoggingActive();
    getMFC1Range();
    getMFC2Range();
    getMFC3Range();
    getMFC4Range();
    getMFC5Range();
    getMFC6Range();
    getMFCRecipeFlow();
    getRecipeMBPosition();
    getRecipeRFPower();   
    getMaxRFPowerForward();
    getAutoMan();
    RunRecipe(false);
    getTemp();
    if (has3AxisBoard) { // these commands go to the three axis board so suppress them if no board attached
        getPHSlitLength();
        getPHSlitWidth();
        getPHSafetyGap();
    }
}

void PlasmaController::resetCTL()
{
    sendCommand("$90%");
    readResponse();
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

// this replaces the collisionlaser()
void PlasmaController::PollForCollision()
{
    if (m_stageCTL.getXAxisError() == 8 || // TODO: replace with symbolic constantsAxisStatusToUI
        m_stageCTL.getYAxisError() == 8 ||
        m_stageCTL.getZAxisError() == 8) {

        // a collision has been detected
        m_collisionDetected = true;
        m_collisionPassed = false;

        Logger::logInfo("Laser Sensor Tripped");
    }
}

void PlasmaController::getFirmwareVersion()
{
    sendCommand("$8F%"); //GET FW VERSION $8F%; resp[!8Fxx#]; xx = hard coded FW rev in Hex
    QString response = readResponse();
    if (response.length() > 3) {
        QString strVar = response.mid(3, 2);
        Logger::logInfo("CTL Firmware Version: " + strVar);
    }
}

void PlasmaController::MBLeft()
{
    sendCommand("$11000032%"); // $110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readResponse();
}

void PlasmaController::MBRight()
{
    sendCommand("$11010032%"); // $110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readResponse();
}

void PlasmaController::howManyMFCs()
{
    sendCommand("$220009%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7,1);
        bool ok = false;
        int numMFCs = StrVar.toInt(&ok);
        if (ok) {
            // resize (shrink) the MFC qlist if needed
            m_mfcs.resize(numMFCs);
            emit setUINumberOfMFCs(numMFCs);
            Logger::logInfo("Number of MFC's: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not set # of MFCs, last requestData: " + getLastCommand());
}

void PlasmaController::getBatchIDLoggingActive() {
    sendCommand("$220011%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, 1);
        bool ok = false;
        int batchLogging = StrVar.toInt(&ok);
        if (ok) {
            bool batchIDLoggingActive = QVariant(batchLogging).toBool();
            if (batchIDLoggingActive) {
                emit batchIDLoggingIsActive();
            }
            Logger::logInfo("Batch Logging active");
        }
    }
    else
        Logger::logCritical("Could Not retrieve Batch Logging, last requestData sent: " + getLastCommand());
}

void PlasmaController::getMFC1Range() {
    sendCommand("$22002D%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
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

void PlasmaController::getMFC2Range() {
    sendCommand("$22002E%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
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

void PlasmaController::getMFC3Range() {
    sendCommand("$22002F%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
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

void PlasmaController::getMFC4Range() {
    sendCommand("$220030%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
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

void PlasmaController::getMFC5Range() {
    sendCommand("$220031%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[4]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 5 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 5 range, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getMFC6Range() {
    sendCommand("$220032%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double mfcRange = StrVar.toDouble(&ok);
        if (ok) {
            m_mfcs[5]->setRange(mfcRange);
            Logger::logInfo("Loaded MFC 6 Range: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve MFC 6 range, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getRecipeMBPosition() {
    sendCommand("$22003D%");
    QString response = readResponse();
    if (response.length() > 8) {
        QString StrVar = response.mid(7, 7);
        bool ok = false;
        double loadedSP = StrVar.toDouble(&ok);
        if (ok) {
            m_tuner.updateRecipePosition(loadedSP);
            Logger::logInfo("Loaded MB Setpoint: " + StrVar + " %");
        }
    }
    else
        Logger::logCritical("Could Not retrieve MB tuner setpoint, last requestData sent: " + getLastCommand() );
}

void PlasmaController::getRecipeRFPower() {
    sendCommand("$22003C%");
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, -1); // return all characters from position 7 to the end
        StrVar = StrVar.removeLast(); // remove the last char
        bool ok = false;
        double rfLoadedSP = StrVar.toDouble(&ok);
        if (ok) {
            m_pwr.updateRecipeWatts(rfLoadedSP);
            Logger::logInfo("Loaded RF Setpoint: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve RF setpoint, last requestData sent: " + getLastCommand());
}

void PlasmaController::getMFCRecipeFlow()
{
    QString response;
    // The absolute Recipe addresses of 1-6
    QStringList mfc  = (QStringList() << "$220036%" << "$220037%" << "$220038%" << "$220039%" << "$22003A%" << "$22003B%");
    for (int i = 0; i < mfc.length(); i++) {
        sendCommand(mfc.at(i));
        response = readResponse();
        extractMFCFlowFromResponse(i, response);
    }
}

void PlasmaController::extractMFCFlowFromResponse(int mfcNum, QString response)
{
    if (response.length() > 8) {
        QString parsedResponse = response.mid(7, response.length() - 9);
        bool ok = false;
        double mfcRecipeFlow = parsedResponse.toDouble(&ok);
        if (ok) {
            m_mfcs.at(mfcNum)->updateRecipeFlow(mfcRecipeFlow);
        }
        else {
            Logger::logInfo("Unable to get MFC Range: check MFC connections");
        }
    }
}

void PlasmaController::getMaxRFPowerForward()
{
    sendCommand("$220017%");
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, -1); // return all characters from position 7 to the end
        StrVar = StrVar.removeLast(); // remove the last char
        bool ok = false;
        int maxRF = StrVar.toInt(&ok);
        if (ok) {
            m_pwr.setMaxForwardWatts(maxRF);
            Logger::logInfo("Loaded max RF Power Forward");
        }
    }
}

void PlasmaController::getAutoMan() {
    sendCommand("$89%"); //GET_AUTO_MAN   $89%; resp [!890p#] p=1 AutoMode, p=0 ManualMode
    QString response = readResponse();
    if (response.length() > 3) {
        QString StrVar = response.mid(3, 2);
        bool ok = false;
        int autoTune = StrVar.toInt(&ok);
        if (ok) {
            m_tuner.updateAutoTune(autoTune);
            Logger::logInfo("Loaded Tuner Auto Setting: " + StrVar);
        }
    }
    else
        Logger::logCritical("Could Not retrieve Auto/Manual setting, last requestData sent: " + getLastCommand());
}

void PlasmaController::getTemp() {
    sendCommand("$8C%"); // GET_TEMP  $8C%: resp[!8Cxx.xx#]; xx.xx = head temp degrees C base 10
    QString response = readResponse();
    if (response.length() > 3) {
        QString StrVar = response.mid(3, 4);
        bool ok = false;
        double headTemp = StrVar.toDouble(&ok);
        if (ok) {
            m_plasmaHead.setTemperature(headTemp);
            Logger::logInfo("Loaded current temperature: " + StrVar);
        }
        else
            Logger::logCritical("Could Not retrieve temperature, last requestData sent: " + getLastCommand() );
    }
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
            m_plasmaHead.setSlitlength(PHSlitLength);
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
            m_plasmaHead.setSlitWidth(PHSlitWidth);
            Logger::logInfo("Plasma Head Slit Width: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Plasma head slit width, last requestData: " + getLastCommand());
}

void PlasmaController::getPHSafetyGap()
{
    sendCommand("$DA542%"); // GET Chuck to Plasma Head safety gap (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSafetyGap = StrVar.toDouble(&ok);
        if (ok) {
            m_plasmaHead.setSafetyGap(PHSafetyGap);
            Logger::logInfo("Plasma safety gap: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Plasma head safety gap, last requestData: " + getLastCommand());
}
