#include "axescontroller.h"
#include <QSerialPort>
#include "settingsdialog.h"
#include <QString>
#include "logger.h"
#include <QTimer>
#include "UtilitiesAndConstants.h"
#include "gamepadcontroller.h"

const int AUX_INPUT_BUFFER_MAX_SIZE = 90;
const int SERIAL_RESPONSE_TIMEOUT = 2000; // timeout waiting for control pcb response (milliseconds)

// file scope
bool joystickOnLast = false;
bool N2OnLast = false;
bool vacOnLast = false;
bool doorsOpenLast = false;
int LEDStatesLast = 0;
double XposLast = 0.0;
double YposLast = 0.0;
double ZposLast = 0.0;
bool axesInitializedLast = true;

AxesController::AxesController(QObject *parent) :
    QObject(parent),
    m_twoSpotXFirstPoint(0),
    m_twoSpotXSecondPoint(0),
    m_twoSpotYFirstPoint(0),
    m_twoSpotYSecondPoint(0),
    m_stageTestZEnabled(true),
    m_detailedStageTestLogEnabled(true),
    m_stageTestXCount(0),
    m_stageTestYCount(0),
    m_stageTestZCount(0),
    m_pSerialInterface(nullptr),
    m_Xaxis(this),
    m_Yaxis(this),
    m_Zaxis(this),
    m_stage(),
    m_axisStatus(),
    m_LEDstates(0),
    m_sameStateXYZ(false),
    m_joystickOn(false),
    m_joyButtonOn(false),
    m_doorsOpen(false),
    m_N2PurgeOn(false),
    m_vacOn(false),
    m_axesInitialized(false),
    m_Xp2Base(0.0),
    m_Yp2Base(0.0),
    m_Zp2Base(0.0),
    m_Xs2PH(0.0),
    m_Ys2PH(0.0)
{
    SetupInitAxesStateMachine();
    SetupHomeAxesStateMachine();
    SetupTwoSpotStateMachine();
    SetupStageTestStateMachine();
}

AxesController::~AxesController()
{
    // cleanup init axes states
    delete m_pInitAxesIdleState;
    delete m_pInitAxesStartupState;
    delete m_pInitAxesWaitForDoneState;
    delete m_pInitAxesInitializedState;

    // cleanup home axes states
    delete m_pHomeAxesStartupState;
    delete m_pHomeAxesWaitParkZState;
    delete m_pHomeAxesHomeXYState;
    delete m_pHomeAxesWaitHomeXYState;
    delete m_pHomeAxesHomeZState;
    delete m_pHomeAxesWaitHomeZState;
    delete m_pHomeAxesShutdownState;
    delete m_pHomeAxesIdleState;
    delete m_pHomeAxisSuperState;

    // cleanup two spot states
    delete m_pTwoSpotStartupState;
    delete m_pTwoSpotGetFirstState;
    delete m_pTwoSpotWaitJoyBtnOffState;
    delete m_pTwoSpotGetSecondState;
    delete m_pTwoSpotWaitJoyBtnOff2State;
    delete m_pTwoSpotShutdownState;
    delete m_pTwoSpotIdleState;
    delete m_pTwoSpotSuperState;

    // cleanup stage test states
    delete m_pStageTestIdleState;
    delete m_pStageTestStartupState;
    delete m_pStageTestMaxXState;
    delete m_pStageTestMinXState;
    delete m_pStageTestMaxYState;
    delete m_pStageTestMinYState;
    delete m_pStageTestShutdownState;
    delete m_pStageTestSuperState;

    m_pSerialInterface = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
// State machine setup
//////////////////////////////////////////////////////////////////////////////////
void AxesController::SetupInitAxesStateMachine()
{
    m_pInitAxesIdleState = new QState();
    m_pInitAxesStartupState = new QState();
    m_pInitAxesWaitForDoneState = new QState();
    m_pInitAxesInitializedState = new QState();

    // construct transitions
    m_pInitAxesIdleState->addTransition(this, &AxesController::ISM_TransitionStartup, m_pInitAxesStartupState);
    m_pInitAxesStartupState->addTransition(this, &AxesController::ISM_TransitionWaitForDone, m_pInitAxesWaitForDoneState);
    m_pInitAxesWaitForDoneState->addTransition(this, &AxesController::ISM_TransitionInitialized, m_pInitAxesInitializedState);
    m_pInitAxesInitializedState->addTransition(this, &AxesController::ISM_TransitionIdle, m_pInitAxesIdleState);
    m_pInitAxesWaitForDoneState->addTransition(this, &AxesController::ISM_TransitionIdle, m_pInitAxesIdleState);
    m_pInitAxesStartupState->addTransition(this, &AxesController::ISM_TransitionIdle, m_pInitAxesIdleState);

    // entry and exit connections
    //connect(m_pInitAxesIdleState, &QState::entered, this, &AxesController::InitIdleOnEntry);

    // add states to the machine
    m_initStateMachine.addState(m_pInitAxesIdleState);
    m_initStateMachine.addState(m_pInitAxesStartupState);
    m_initStateMachine.addState(m_pInitAxesWaitForDoneState);
    m_initStateMachine.addState(m_pInitAxesInitializedState);

    // set initial state
    m_initStateMachine.setInitialState(m_pInitAxesIdleState);

    // start the state machine
    m_initStateMachine.start();
}

void AxesController::SetupHomeAxesStateMachine()
{
    m_pHomeAxisSuperState = new QState();
    m_pHomeAxesStartupState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesWaitParkZState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesHomeXYState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesWaitHomeXYState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesHomeZState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesWaitHomeZState = new QState(m_pHomeAxisSuperState);
    m_pHomeAxesShutdownState = new QState();
    m_pHomeAxesIdleState = new QState();

    // set super state machine initial state and add it to the state machine
    m_pHomeAxisSuperState->setInitialState(m_pHomeAxesStartupState);
    m_homeStateMachine.addState(m_pHomeAxisSuperState);

    // add the rest of the states
    m_homeStateMachine.addState(m_pHomeAxesShutdownState);
    m_homeStateMachine.addState(m_pHomeAxesIdleState);

    // add transitions to and from the super state
    m_pHomeAxisSuperState->addTransition(this, &AxesController::HSM_TransitionIdle, m_pHomeAxesIdleState);
    m_pHomeAxisSuperState->addTransition(this, &AxesController::HSM_TransitionShutdown, m_pHomeAxesShutdownState);
    m_pHomeAxesIdleState->addTransition(this, &AxesController::HSM_TransitionStartup, m_pHomeAxisSuperState);

    // add the rest of the transitions
    m_pHomeAxesStartupState->addTransition(this, &AxesController::HSM_TransitionStartupToWaitParkZ, m_pHomeAxesWaitParkZState);
    m_pHomeAxesWaitParkZState->addTransition(this, &AxesController::HSM_TransitionWaitParkZToHomeXY, m_pHomeAxesHomeXYState);
    m_pHomeAxesHomeXYState ->addTransition(this, &AxesController::HSM_TransitionHomeXYToWaitHomeXY, m_pHomeAxesWaitHomeXYState);
    m_pHomeAxesWaitHomeXYState->addTransition(this, &AxesController::HSM_TransitionHomeXYToHomeZ, m_pHomeAxesHomeZState);
    m_pHomeAxesHomeZState->addTransition(this, &AxesController::HSM_TransitionHomeZToWaitHomeZ, m_pHomeAxesWaitHomeZState);
    m_pHomeAxesShutdownState->addTransition(this, &AxesController::HSM_TransitionIdle, m_pHomeAxesIdleState);
    m_pHomeAxesIdleState->addTransition(this, &AxesController::HSM_TransitionShutdown, m_pHomeAxesShutdownState);

    // entry and exit connections
    //connect(m_pHomeAxesIdleState, &QState::entered, this, &AxesController::HomeIdleOnEntry);

    // set initial state
    m_homeStateMachine.setInitialState(m_pHomeAxesIdleState);

    // start the state machine
    m_homeStateMachine.start();
}

void AxesController::SetupTwoSpotStateMachine()
{
    m_pTwoSpotSuperState = new QState();
    m_pTwoSpotStartupState = new QState(m_pTwoSpotSuperState);
    m_pTwoSpotGetFirstState = new QState(m_pTwoSpotSuperState);
    m_pTwoSpotWaitJoyBtnOffState = new QState(m_pTwoSpotSuperState);
    m_pTwoSpotGetSecondState = new QState(m_pTwoSpotSuperState);
    m_pTwoSpotWaitJoyBtnOff2State = new QState(m_pTwoSpotSuperState);
    m_pTwoSpotShutdownState = new QState();
    m_pTwoSpotIdleState = new QState();

    // set super state machine initial state and add it to the state machine
    m_pTwoSpotSuperState->setInitialState(m_pTwoSpotStartupState);
    m_twoSpotStateMachine.addState(m_pTwoSpotSuperState);

    // add the rest of the states
    m_twoSpotStateMachine.addState(m_pTwoSpotShutdownState);
    m_twoSpotStateMachine.addState(m_pTwoSpotIdleState);

    // add transitions to and from the super state
    m_pTwoSpotSuperState->addTransition(this, &AxesController::TSSM_TransitionIdle, m_pTwoSpotIdleState);
    m_pTwoSpotSuperState->addTransition(this, &AxesController::TSSM_TransitionShutdown, m_pTwoSpotShutdownState);
    m_pTwoSpotIdleState->addTransition(this, &AxesController::TSSM_TransitionStartup, m_pTwoSpotSuperState);

    // add the rest of the transitions
    m_pTwoSpotStartupState->addTransition(this, &AxesController::TSSM_TransitionGetFirst, m_pTwoSpotGetFirstState);
    m_pTwoSpotGetFirstState->addTransition(this, &AxesController::TSSM_TransitionJoyBtnOff, m_pTwoSpotWaitJoyBtnOffState);
    m_pTwoSpotWaitJoyBtnOffState ->addTransition(this, &AxesController::TSSM_TransitionGetSecond, m_pTwoSpotGetSecondState);
    m_pTwoSpotGetSecondState->addTransition(this, &AxesController::TSSM_TransitionJoyBtnOff2, m_pTwoSpotWaitJoyBtnOff2State);
    m_pTwoSpotShutdownState->addTransition(this, &AxesController::TSSM_TransitionIdle, m_pTwoSpotIdleState);

    // entry and exit connections
    //connect(m_pTwoSpotIdleState, &QState::entered, this, &AxesController::TwoSpotIdleOnEntry);

    // set initial state
    m_twoSpotStateMachine.setInitialState(m_pTwoSpotIdleState);

    // start the state machine
    m_twoSpotStateMachine.start();
}

void AxesController::SetupStageTestStateMachine()
{
    // stage test state machine
    m_pStageTestSuperState = new QState();
    m_pStageTestStartupState = new QState(m_pStageTestSuperState);
    m_pStageTestMaxXState = new QState(m_pStageTestSuperState);
    m_pStageTestMinXState = new QState(m_pStageTestSuperState);
    m_pStageTestMaxYState = new QState(m_pStageTestSuperState);
    m_pStageTestMinYState = new QState(m_pStageTestSuperState);
    m_pStageTestIdleState = new QState();
    m_pStageTestShutdownState = new QState();

    // set super state machine initial state and add it to the state machine
    m_pStageTestSuperState->setInitialState(m_pStageTestStartupState);
    m_stageTestStateMachine.addState(m_pStageTestSuperState);

    // add the rest of the states
    m_stageTestStateMachine.addState(m_pStageTestShutdownState);
    m_stageTestStateMachine.addState(m_pStageTestIdleState);

    // add transitions to and from the super state
    m_pStageTestSuperState->addTransition(this, &AxesController::STSM_TransitionIdle, m_pStageTestIdleState);
    m_pStageTestSuperState->addTransition(this, &AxesController::STSM_TransitionShutdown, m_pStageTestShutdownState);
    m_pStageTestIdleState->addTransition(this, &AxesController::STSM_TransitionStartup, m_pStageTestSuperState);

    // add the rest of the transitions
    m_pStageTestStartupState->addTransition(this, &AxesController::STSM_TransitionMaxX, m_pStageTestMaxXState);
    m_pStageTestMaxXState->addTransition(this, &AxesController::STSM_TransitionMaxY, m_pStageTestMaxYState);
    m_pStageTestMaxYState ->addTransition(this, &AxesController::STSM_TransitionMinX, m_pStageTestMinXState);
    m_pStageTestMinXState->addTransition(this, &AxesController::STSM_TransitionMinY, m_pStageTestMinYState);
    m_pStageTestMinYState->addTransition(this, &AxesController::STSM_TransitionMaxX, m_pStageTestMaxXState);
    m_pStageTestShutdownState->addTransition(this, &AxesController::STSM_TransitionIdle, m_pStageTestIdleState);

    // set initial state
    m_stageTestStateMachine.setInitialState(m_pStageTestIdleState);

    // start the state machine
    m_stageTestStateMachine.start();
}

//////////////////////////////////////////////////////////////////////////////////
// Serial Port
//////////////////////////////////////////////////////////////////////////////////

bool AxesController::sendCommand(const QString& command)
{
    return m_pSerialInterface->sendCommand(command);
}

// read a single response
QString AxesController::readResponse()
{
    return m_pSerialInterface->readResponse();
}

QString AxesController::getPortErrorString()
{
    return m_pSerialInterface->errorString();
}

bool AxesController::isOpen()
{
    return m_pSerialInterface->isOpen();
}

void AxesController::setSerialInterface(SerialInterface *interface)
{
    // cleanup shared serial resource
    if (m_pSerialInterface != nullptr) {
        if (m_pSerialInterface->isOpen()) {
            m_pSerialInterface->close();
        }
        delete m_pSerialInterface;
        m_pSerialInterface = nullptr;
    }
    m_pSerialInterface = interface;
}


//////////////////////////////////////////////////////////////////////////////////
// State machines
//////////////////////////////////////////////////////////////////////////////////
void AxesController::RunInitAxesSM()
{
    if (m_initStateMachine.configuration().contains(m_pInitAxesStartupState)) { // in Startup state

        //SM set to idle
        emit TSSM_TransitionIdle(); // two spot state machine to idle
        emit HSM_TransitionIdle(); // home state machine to idel
        emit ScanSM_TransitionIdle(); // scan state machine to idle

        //send commands
        stopMotors();
        setValve2Off();
        sendInitCMD();

        // update UI
        emit stageStatusUpdate("Initializing Axes", "");
        emit initSMStartup();

        Logger::logInfo("Initializing Axes");

        // initiate transition to WaitForDone state
        emit ISM_TransitionWaitForDone();
    }
    else if (m_initStateMachine.configuration().contains(m_pInitAxesWaitForDoneState)) { // in WaitForDone state
        if (nextStateReady()) {
            // initiate transtion to Initialized state
            emit ISM_TransitionInitialized();
        }
    }
    else if (m_initStateMachine.configuration().contains(m_pInitAxesIdleState)) { // in Idle state

        // no op
    }
    else if (m_initStateMachine.configuration().contains(m_pInitAxesInitializedState)) { // in initialized state
        // retain the initilized state
        m_axesInitialized = true;

        // update UI
        emit stageStatusUpdate("Stage Initialized", "");
        emit initSMDone();


        Logger::logInfo("Stage Initialized");

        // initiate transtion to Idle state
        emit ISM_TransitionIdle();
    }
}

void AxesController::RunHomeAxesSM()
{
    //Start by parking Z in a safe position to move
    if (m_homeStateMachine.configuration().contains(m_pHomeAxesStartupState)) { // in Startup state

        if (nextStateReady()) {

            // put init axis state machine in the Idle state
            emit ISM_TransitionIdle();

            // update UI
            emit setUIHomeSMStartup();
            emit stageStatusUpdate("Homing Startup - Parking Z", "Homing X & Y");

            move(ZAXIS_COMMAND_NUM, m_Zaxis.getMaxSpeed(), m_stage.getPinsBuriedPos());
            Logger::logInfo("Homing Start");

            QString logStr = "Move Z at: " + getZMaxSpeedQStr() + " /sec ";
            Logger::logInfo(logStr + "to: " + QString::number(m_stage.getPinsBuriedPos(), 'f', 2));

            // transition to wait_park_z
            emit HSM_TransitionStartupToWaitParkZ();
        }
    }
    // wait until z is parked
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesWaitParkZState)) { // in wait park z state

        if (nextStateReady()) {

            // transition to home xy
            emit HSM_TransitionWaitParkZToHomeXY();

            Logger::logInfo("Homing Z Parked");
        }
    }
    // set speeds
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesHomeXYState)) { // in home xy state

        // update the UI
        emit stageStatusUpdate("Homing X & Y", "Homing Z");

        Logger::logInfo("Homing X & Y");
        Logger::logInfo("Move X Speed: " + m_Xaxis.getMaxSpeedQStr() + " /sec " + "Move Y Speed: " +
                        m_Yaxis.getMaxSpeedQStr() + " /sec");

        move(XAXIS_COMMAND_NUM, m_Xaxis.getMaxSpeed(), m_Xaxis.getHomePos());
        move(YAXIS_COMMAND_NUM, m_Yaxis.getMaxSpeed(), m_Yaxis.getHomePos());

        Logger::logInfo("X to: " + m_Xaxis.getHomePosQStr() + " Y to: " + m_Yaxis.getHomePosQStr());

        // transition to wait home XY
        emit HSM_TransitionHomeXYToWaitHomeXY();
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesWaitHomeXYState)) { // in wait home xy state

        if (nextStateReady()) {

            // transition to home Z
            emit HSM_TransitionHomeXYToHomeZ();

            Logger::logInfo("X & Y Homed");
        }
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesHomeZState)) { // in home z state

        // update the UI
        emit stageStatusUpdate("Homing Z", "");

        Logger::logInfo("Homing Z");

        move(ZAXIS_COMMAND_NUM, m_Zaxis.getMaxSpeed(), m_stage.getPinsBuriedPos());

        Logger::logInfo("Z to " + QString::number(m_stage.getPinsBuriedPos(), 'f', 2));

        // transition to wait home Z
        emit HSM_TransitionHomeZToWaitHomeZ();
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesWaitHomeZState)) { // in wait home z state

        // transition to wait home Z
        emit HSM_TransitionHomeZToWaitHomeZ();

        if (nextStateReady()) {

            // update the UI
            emit stageStatusUpdate("", "");
            emit setUIHomeSMDone();

            Logger::logInfo("Z Homed");

            // transition to idle
            emit HSM_TransitionIdle();
        }
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesShutdownState)) { // in shutdown state

        // update the UI
        emit stageStatusUpdate("Operator Abort Homing", "");

        Logger::logInfo("Operator Abort Homing");

        stopMotors();

        move(ZAXIS_COMMAND_NUM, m_Zaxis.getMaxSpeed(), m_stage.getPinsBuriedPos());

        // transition to idle
        emit HSM_TransitionIdle();

        // update UI
        emit stageStatusUpdate("", "");

        // tell the front end we are done
        emit setUIHomeSMDone();

    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesIdleState)) { // in idle state
        // no op
    }
}

void AxesController::RunTwoSpotSM()
{
    if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotStartupState)) {

        toggleJoystickOn();

        // disable other state machines if they are active
        emit HSM_TransitionIdle();
        emit ScanSM_TransitionIdle();

        // update UI
        emit setUITwoSpotSMStartup();
        emit stageStatusUpdate("Use Controller for Stage", "Spot First Point");

        Logger::logInfo("TwoSpotSM Start Up");

        emit TSSM_TransitionGetFirst();
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotGetFirstState)) {

        if (m_joystickOn && m_joyButtonOn) {

            emit stageStatusUpdate("Use Controller for Stage", "Release JoyStick Button");

            // translate into ph coordinates and save
            m_twoSpotXFirstPoint = TranslateCoordXBase2PH(m_Xaxis.getPosition());
            m_twoSpotYFirstPoint = TranslateCoordLaserY2PH(m_Yaxis.getPosition());

            Logger::logInfo("TwoSpotSM Got First");

            emit TSSM_TransitionJoyBtnOff();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotWaitJoyBtnOffState)) {

        if (m_joystickOn && !m_joyButtonOn) {

            Logger::logInfo("TwoSpotSM Getting Second");

            emit stageStatusUpdate("Use Controller for Stage", "Spot Second Point");

            emit TSSM_TransitionGetSecond();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotGetSecondState)) {

        if (m_joystickOn && m_joyButtonOn) {

            emit stageStatusUpdate("Use Controller for Stage", "Release JoyStick Button");

            // translate to ph coordinates and save
            m_twoSpotXSecondPoint = TranslateCoordXBase2PH(m_Xaxis.getPosition());
            m_twoSpotYSecondPoint = TranslateCoordLaserY2PH(m_Yaxis.getPosition());

            //determine box orientation and corners for scanning
            checkAndSetXDimensions();
            checkAndSetYDimensions();

            Logger::logInfo("TwoSpotSM Got Second - done");

            emit TSSM_TransitionJoyBtnOff2();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotWaitJoyBtnOff2State)) {

        if (m_joystickOn && !m_joyButtonOn) {

            emit TSSM_TransitionShutdown();
        }

    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotShutdownState)) {

        toggleJoystickOff();

        emit TSSM_TransitionIdle();

        // update UI
        emit stageStatusUpdate("", "");

        // tell the front end we are done
        emit setUITwoSpotSMDone();

    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotIdleState)) {
        // no op
    }
}

void AxesController::RunStageTestSM()
{
    if (m_stageTestStateMachine.configuration().contains(m_pStageTestStartupState)) {

        // disable other state machines if they are active
        emit TSSM_TransitionIdle();
        emit HSM_TransitionIdle();
        emit ScanSM_TransitionIdle();

        // reset counters
        m_stageTestXCount = 0;
        m_stageTestYCount = 0;
        m_stageTestZCount = 0;

        // update UI
        emit STSM_TransitionMaxX();

        Logger::logInfo("Stage Test Started");
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestMaxXState)) {

        if (nextStateReady()) {

            Logger::logInfo("Stage Test - Max X");

            // set in constructor
            if (m_detailedStageTestLogEnabled)
                Logger::logInfo("X movements: " + QString::number(m_stageTestXCount));

            // update UI
            emit stageStatusUpdate("X axis move to: " + m_Xaxis.getMaxPosQStr() + " at speed: " + m_Xaxis.getMaxSpeedQStr(), "");

            // set speed and intiate move
            move(XAXIS_COMMAND_NUM, m_Xaxis.getMaxSpeed(), m_Xaxis.getMaxPos());
            m_stageTestXCount++;

            // set in constructor
            if (m_stageTestZEnabled)
                stageTestZMax();

            emit STSM_TransitionMaxY();
        }
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestMaxYState)) {

        if (nextStateReady()) {

            Logger::logInfo("Stage Test - Max Y");

            // set in constructor
            if (m_detailedStageTestLogEnabled)
                Logger::logInfo("Y movements: " + QString::number(m_stageTestYCount));

            // update UI
            emit stageStatusUpdate("Y axis move to: " + m_Yaxis.getMaxPosQStr() + " at speed: " + m_Yaxis.getMaxSpeedQStr(), "");

            // set speed and intiate move
            move(YAXIS_COMMAND_NUM, m_Yaxis.getMaxSpeed(), m_Yaxis.getMaxPos());
            m_stageTestYCount++;

            // set in constructor
            if (m_stageTestZEnabled)
                stageTestZMin();

            emit STSM_TransitionMinX();
        }
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestMinXState)) {

        if (nextStateReady()) {

            Logger::logInfo("Stage Test - Min X");

            // set in constructor
            if (m_detailedStageTestLogEnabled)
                Logger::logInfo("X movements: " + QString::number(m_stageTestXCount));

            // update UI
            emit stageStatusUpdate("X axis move to: 0 at speed: " + m_Xaxis.getMaxSpeedQStr(), "");

            // set speed and intiate move
            move(XAXIS_COMMAND_NUM, m_Xaxis.getMaxSpeed(), 0);
            m_stageTestXCount++;

            // set in constructor
            if (m_stageTestZEnabled)
                stageTestZMax();

            emit STSM_TransitionMinY();
        }
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestMinYState)) {

        if (nextStateReady()) {

            Logger::logInfo("Stage Test - Min Y");

            // set in constructor
            if (m_detailedStageTestLogEnabled)
                Logger::logInfo("Y movements: " + QString::number(m_stageTestYCount));

            // update UI
            emit stageStatusUpdate("Y axis move to: 0 at speed: " + m_Yaxis.getMaxSpeedQStr(), "");

            // set speed and intiate move
            move(YAXIS_COMMAND_NUM, m_Yaxis.getMaxSpeed(), 0);
            m_stageTestYCount++;

            // set in constructor
            if (m_stageTestZEnabled)
                stageTestZMin();

            emit STSM_TransitionMaxX();
        }
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestShutdownState)) {

        Logger::logInfo("Stage Test Shutting Down");

        // update UI
        emit stageStatusUpdate("", "");

        stopAllMotors();

        emit STSM_TransitionIdle();
    }
    else if (m_stageTestStateMachine.configuration().contains(m_pStageTestIdleState)) {
        // no op
    }
}

void AxesController::stageTestZMax()
{
    Logger::logInfo("Stage Test - Max Z");

    // set in constructor
    if (m_detailedStageTestLogEnabled)
        Logger::logInfo("Z movements: " + QString::number(m_stageTestZCount));

    // update UI
    emit stageStatusUpdate("Z axis move to: " + m_Zaxis.getMaxPosQStr() + " at speed: " + m_Zaxis.getMaxSpeedQStr(), "");

    // set speed and intiate move
    move(ZAXIS_COMMAND_NUM, m_Zaxis.getMaxSpeed(), m_Zaxis.getMaxPos());

    m_stageTestZCount++;
}

void AxesController::stageTestZMin()
{
    Logger::logInfo("Stage Test - Min Z");

    // set in constructor
    if (m_detailedStageTestLogEnabled)
        Logger::logInfo("Z movements: " + QString::number(m_stageTestZCount));

    // update UI
    emit stageStatusUpdate("Z axis move to: 0 at speed: " + m_Zaxis.getMaxSpeedQStr(), "");

    // set speed and intiate move
    move(ZAXIS_COMMAND_NUM, m_Zaxis.getMaxSpeed(), 0);

    m_stageTestZCount++;
}

// returns true if any state machine is active (non idle state)
bool AxesController::axisStateMachineActive()
{
    if (m_initStateMachine.configuration().contains(m_pInitAxesIdleState) &&
        m_homeStateMachine.configuration().contains(m_pHomeAxesIdleState) &&
        m_twoSpotStateMachine.configuration().contains(m_pTwoSpotIdleState) &&
        m_stageTestStateMachine.configuration().contains(m_pStageTestIdleState)) {
        return false;
    }
    else {
        return true;
    }
}

void AxesController::checkAndSetXDimensions()
{
    double min = 0, max = 0;

    if (m_twoSpotXFirstPoint > m_twoSpotXSecondPoint) {
        min = m_twoSpotXSecondPoint;
        max = m_twoSpotXFirstPoint;
    }
    else {
        min = m_twoSpotXFirstPoint;
        max = m_twoSpotXSecondPoint;
    }
    emit xLimitsChanged(min, max);
}

void AxesController::checkAndSetYDimensions()
{
    double min = 0, max = 0;

    if (m_twoSpotYFirstPoint > m_twoSpotYSecondPoint) {
        min = m_twoSpotYSecondPoint;
        max = m_twoSpotYFirstPoint;
    }
    else {
        min = m_twoSpotYFirstPoint;
        max = m_twoSpotYSecondPoint;
    }
    emit yLimitsChanged(min, max);
}

void AxesController::setSameStateXYZsame()
{
    if ((m_Xaxis.getCurrentState() == m_Yaxis.getCurrentState()) && (m_Xaxis.getCurrentState() == m_Zaxis.getCurrentState())) {
        m_sameStateXYZ = true;
    }
    else {
        m_sameStateXYZ = false;
    }
}

bool AxesController::nextStateReady()
{
    if ((m_sameStateXYZ == true) && (m_Xaxis.getCurrentState() >= AXIS_IDLE)) {
        return true;
    }

    return false;
}

void AxesController::gameControllerMove(int axis, int pct)
{
    // $AB0xss.s%; resp [!AB0xss.s#] where 0x = axis number, ss.s is plus/minus percent of max joy speed
    QString joyMoveCmd = "$AB0";

    // map to our axis notation
    switch(axis) {
    case GamepadController::CONTROLLER_AXIS_LEFTX: // maps to X axis
        joyMoveCmd += QString::number(XAXIS_COMMAND_NUM);
        break;
    case GamepadController::CONTROLLER_AXIS_LEFTY: // maps to Y axis
        joyMoveCmd += QString::number(YAXIS_COMMAND_NUM);
        break;
    case GamepadController::CONTROLLER_AXIS_RIGHTY: // maps to Z axis
        joyMoveCmd += QString::number(ZAXIS_COMMAND_NUM);
        break;
    }
    //joyMoveCmd = "$AB011000%";
    joyMoveCmd += QString::number(pct) + "%";
    sendCommand(joyMoveCmd);
    readResponse();
    //Logger::logInfo("gameControllerMove(" + QString::number(axis) + ", " + QString::number(pct) + ")");
}

void AxesController::setValve2On()
{
    sendCommand("$C701%"); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
    readResponse();
}

void AxesController::setValve2Off()
{
    sendCommand("$C700%"); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
    readResponse();
}

void AxesController::stopMotors()
{
    sendCommand("$B3%"); //stop all motors
    readResponse();
}

void AxesController::sendInitCMD()
{
    sendCommand("$B500%"); //start initializing X axis
    readResponse();
    sendCommand("$B501%"); //start initializing Y axis
    readResponse();
    sendCommand("$B502%"); //start initializing Z axis
    readResponse();
}

void AxesController::AxisStartup()
{
    getFirmwareVersion();
    getXMaxPosition();
    getYMaxPosition();
    getZMaxPosition();
    getXMaxSpeed();
    getYMaxSpeed();
    getZMaxSpeed();
    getXp2Base();
    getYp2Base();
    getZp2Base();
    getXs2PH();
    getYs2PH();
    getZPinsBuried();
    getZPinsExposed();
    getLoadX2Base();
    getLoadY2Base();
    getLoadZ2Base();
    setAxisStateMachinesIdle();
}

void AxesController::parseStatus(QString serialResponse)
{
    QString statusToParse = serialResponse;
    statusToParse = statusToParse.mid(3); //remove the first 3 chars
    statusToParse = statusToParse.mid(0, statusToParse.length() -1); //remove # from response before split
    m_axisStatus = statusToParse.split(QLatin1Char(';')); //split string up

    updateAxisStatus();
}
void AxesController::getAxisStatus()
{
    sendCommand("$C0%"); //GET_STATUS $C0%; resp[!C0nn;nn;X state;X error;X pos; Y state; Y error, Y pos; Z state; Z error; Z pos#]
    QString response = readResponse();
    if (response.length() < 30) Logger::logCritical("Bad axis status response = " + response);
    parseStatus(response);
}


void AxesController::setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble)
{
    bool ok;
    m_LEDstates = firstHexStrNibble.toInt(&ok, 16); //First byte
    m_LEDstates = m_LEDstates<<8;
    m_LEDstates += secondHexStrNibble.toInt(&ok, 16); //First byte

    if (m_LEDstates != LEDStatesLast) {
        LEDStatesLast = m_LEDstates;
        Logger::logDebug("Axis CTL LEDstates:" + QString::number(m_LEDstates, 2));
    }
}

void AxesController::updateAxisStatus()
{
    if (m_axisStatus.length() == 11) {

        if (m_axisStatus[0] == "C0")
            m_joyButtonOn = true;
        else
            m_joyButtonOn = false;

        //handle LED status
        setLEDstate(m_axisStatus[0], m_axisStatus[1]);

        m_Xaxis.setCurrentState(m_axisStatus[2]);
        m_Xaxis.setCurrentError(m_axisStatus[3]);
        m_Xaxis.setCurrentPosition(m_axisStatus[4]);
        //Y
        m_Yaxis.setCurrentState(m_axisStatus[5]);
        m_Yaxis.setCurrentError(m_axisStatus[6]);
        m_Yaxis.setCurrentPosition(m_axisStatus[7]);
        //Z
        m_Zaxis.setCurrentState(m_axisStatus[8]);
        m_Zaxis.setCurrentError(m_axisStatus[9]);
        m_Zaxis.setCurrentPosition(m_axisStatus[10]);
        //XYZ same? (probably stopped)
        setSameStateXYZsame();

        // Check various status bits
        doorsStatus();
        //joyBtnStatus();
        vacStatus();
        N2PurgeStatus();

        //log any change
        checkAndLogAxesStatusChange();
    }
}

// currently logging positional changes only
void AxesController::checkAndLogAxesStatusChange()
{
    bool axisPositionChanged = false;
    bool axisInitStateChanged = false;
    bool XaxisInitialized = false;
    bool YaxisInitialized = false;
    bool ZaxisInitialized = false;

    // Look for Xaxis state or position change
    if (getXAxisState() >= AXIS_IDLE) {
        XaxisInitialized = true;
        if (m_Xaxis.getPosition() != XposLast) {
            XposLast = m_Xaxis.getPosition();
            axisPositionChanged = true;
        }
    }
    else {
        XaxisInitialized = false;
    }
    // Look for Yaxis state or position change
    if (getYAxisState() >= AXIS_IDLE) {
        YaxisInitialized = true;
        if (m_Yaxis.getPosition() != YposLast) {
            YposLast = m_Yaxis.getPosition();
            axisPositionChanged = true;
        }
    }
    else {
        XaxisInitialized = false;
    }
    // Look for Zaxis state or position change
    if (getZAxisState() >= AXIS_IDLE) {
        ZaxisInitialized = true;
        if (m_Zaxis.getPosition() != ZposLast) {
            ZposLast = m_Zaxis.getPosition();
            axisPositionChanged = true;
        }
    }
    else {
        ZaxisInitialized = false;
    }
    // If any axis is uninitialized then unset the flag
    if (!XaxisInitialized || !YaxisInitialized || !ZaxisInitialized) {
        m_axesInitialized = false;
    }
    // look for an axes initialized change
    if (m_axesInitialized != axesInitializedLast) {
        axesInitializedLast = m_axesInitialized;
        axisInitStateChanged = true;
    }
    // Look for an axis position change
    if (axisPositionChanged) {
        // log the new position
        Logger::logInfo(QString("Stage Xpos: %1 Ypos: %2 Zpos: %3").
                        arg(m_Xaxis.getPositionQStr(), m_Yaxis.getPositionQStr(), m_Zaxis.getPositionQStr()));
    }
    // trigger a ui update
    if (axisPositionChanged || axisInitStateChanged) {
        emit updateUIAxisStatus();
    }
}


void AxesController::doorsStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_DOOR_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_doorsOpen = isBitSet(m_LEDstates, bit);

        if (m_doorsOpen != doorsOpenLast) {
            doorsOpenLast = m_doorsOpen;

            if (m_doorsOpen)
                Logger::logInfo("Doors : open");
            else
                Logger::logInfo("Doors : closed");
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_DOOR_STATUS_BIT));
    }
}

/*void AxesController::joyBtnStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_JOY_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_joystickOn = isBitSet(m_LEDstates, bit);
        Logger::logInfo(QString::number(m_LEDstates));

        if (m_joystickOn != joystickOnLast) {
            joystickOnLast = m_joystickOn;

            if (m_joystickOn)
                Logger::logInfo("Joystick : enabled");
            else
                Logger::logInfo("Joystick : disabled");

            emit joystickStateChanged(m_joystickOn);
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_JOY_STATUS_BIT));
    }
}*/

void AxesController::vacStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_VAC_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_vacOn = isBitSet(m_LEDstates, bit);

        if (m_vacOn != vacOnLast) {
            vacOnLast = m_vacOn;

            if (m_vacOn)
                Logger::logInfo("Vac : enabled");
            else
                Logger::logInfo("Vac : disabled");

            emit vacStateChanged(m_vacOn);
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_VAC_STATUS_BIT));
    }
}

void AxesController::N2PurgeStatus()
{
    bool ok = false;
    int bit = m_config.getValueForKey(CONFIG_N2PURGE_STATUS_BIT).toInt(&ok);

    if (ok) {
        m_N2PurgeOn = isBitSet(m_LEDstates, bit);

        if (m_N2PurgeOn != N2OnLast) {
            N2OnLast = m_N2PurgeOn;

            if (m_N2PurgeOn)
                Logger::logInfo("N2 Purge : enabled");
            else
                Logger::logInfo("N2 Purge : disabled");

            emit n2StateChanged(m_N2PurgeOn);
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(CONFIG_N2PURGE_STATUS_BIT));
    }
}

void AxesController::move(int axisCommandNum, float speed, float position)
{
    setAxisSpeed(axisCommandNum, speed);
    moveAxisAbsolute(axisCommandNum, position);

    Logger::logDebug("Move " + QString::number(axisCommandNum) + " to: " + QString::number(position, 'f', 2) + " at speed: " + QString::number(speed, 'f', 2));
}

void AxesController::moveAxisAbsolute(int axisCommandNum, float targetPosition)
{
    if (axisCommandNum >= 0 && axisCommandNum <= 2) {
        QString command = "$B60" + QString::number(axisCommandNum);
        command += QString::number(targetPosition, 'f', 2); // number, 6=total field length including decimal point, 'f'=dont use scientific notation, 2=number of decimal places, padding char
        command += "%";
        sendCommand(command); // ABS_MOVE $B60xaa.aa%; resp [!B60xaa.aa#] 0x = axis num, aa.aa = destination in mm (float)
        readResponse();
    }
    else {
        Logger::logCritical("Bad axis command number sent to movAxisAbsolute: " + QString::number(axisCommandNum));
    }
}

void AxesController::setAxisSpeed(int axisCommandNum, float targetSpeed)
{
    if (axisCommandNum >= 0 && axisCommandNum <= 2) {
        QString command = "$B40" + QString::number(axisCommandNum); // SET_SPEED  $B40xss.ss%; resp [!B40xss.ss#] 0x = axis number, ss.ss = mm/sec (float)
        command += QString::number(targetSpeed, 'f', 2); // number, 6=total field length including decimal point, 'f'=dont use scientific notation, 2=number of decimal places, padding char
        command += "%";
        sendCommand(command);
        readResponse();
    }
    else {
        Logger::logCritical("Bad axis command number sent to setAxisSpeed: " + QString::number(axisCommandNum));
    }
}

void AxesController::stopAllMotors()
{
    sendCommand("$B3%"); // stop any motors in motion
    readResponse();
}

void AxesController::getFirmwareVersion()
{
    sendCommand("$A1%"); //GET FW VERSION $A1% resp[!A1xx#]; xx = hard coded FW rev in Hex
    QString response = readResponse();
    if (response.length() > 3) {
        QString strVar = response.mid(3, 2);
        Logger::logInfo("AUX Firmware Version: " + strVar);
    }
}

void AxesController::getXMaxSpeed()
{
    sendCommand("$DA107%"); //GET_X_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxSpeed = StrVar.toDouble(&ok);
        if (ok) {
            m_Xaxis.saveMaxSpeed(maxSpeed);
            Logger::logInfo("X Max Speed: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max speed for X, last requestData: " + getLastCommand());
}

void AxesController::getYMaxSpeed()
{
    sendCommand("$DA207%"); //GET_Y_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxSpeed = StrVar.toDouble(&ok);
        if (ok) {
            m_Yaxis.saveMaxSpeed(maxSpeed);
            Logger::logInfo("Y Max Speed: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max speed for Y, last requestData: " + getLastCommand());
}
void AxesController::getZMaxSpeed()
{
    sendCommand("$DA307%"); //GET_Z_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxSpeed = StrVar.toDouble(&ok);
        if (ok) {
            m_Zaxis.saveMaxSpeed(maxSpeed);
            Logger::logInfo("Z Max Speed: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max speed for Z, last requestData: " + getLastCommand());
}

void AxesController::getXMaxPosition()
{
    sendCommand("$DA106%"); // GET_X_MAX_POSITION  X maximum allowed position in MM (float) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxPos = StrVar.toDouble(&ok);
        if (ok) {
            m_Xaxis.saveMaxPos(maxPos);
            Logger::logInfo("X Max Pos: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max position for X, last requestData: " + getLastCommand());
}

void AxesController::getYMaxPosition()
{
    sendCommand("$DA206%"); // GET_Y_MAX_POSITION Y maximum allowed position in MM (float) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxPos = StrVar.toDouble(&ok);
        if (ok) {
            m_Yaxis.saveMaxPos(maxPos);
            Logger::logInfo("Y Max Pos: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max position for Y, last requestData: " + getLastCommand());
}

void AxesController::getZMaxPosition()
{
    sendCommand("$DA306%"); // GET_Z_MAX_POSITION  Z maximum allowed position in MM (float) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double maxPos = StrVar.toDouble(&ok);
        if (ok) {
            m_Zaxis.saveMaxPos(maxPos);
            Logger::logInfo("Z Max Pos: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get max position for Z, last requestData: " + getLastCommand());
}

void AxesController::getXp2Base()
{
    sendCommand("$DA510%"); //GET Xp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        m_Xp2Base = StrVar.toDouble(&ok);
        if (ok) {
            Logger::logInfo("Xp to Base: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get X relative to Base, last requestData: " + getLastCommand());
}
void AxesController::getYp2Base()
{
    sendCommand("$DA520%"); //GET Yp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        m_Yp2Base = StrVar.toDouble(&ok);
        if (ok) {
            Logger::logInfo("Yp to Base: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get Y relative to Base, last requestData: " + getLastCommand());
}
void AxesController::getZp2Base()
{
    sendCommand("$DA530%"); //GET Zp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        m_Zp2Base = StrVar.toDouble(&ok);
        if (ok) {
            Logger::logInfo("Zp to Base: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get Z relative to Base, last requestData: " + getLastCommand());
}

// laser x to base coordinates
void AxesController::getXs2PH()
{
    sendCommand("$DA511%"); //GET Xs_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        m_Xs2PH = StrVar.toDouble(&ok);
        if (ok) {
            Logger::logInfo("Xs to Plasma Head: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get X relative to Plasma head, last requestData: " + getLastCommand());
}

// laser y to base coordinates
void AxesController::getYs2PH()
{
    sendCommand("$DA521%"); //GET Ys_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        m_Ys2PH = StrVar.toDouble(&ok);
        if (ok) {
            Logger::logInfo("Ys to Plasma Head: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get Y relative to Plasma head, last requestData: " + getLastCommand());
}


void AxesController::getZPinsBuried()
{
    sendCommand("$DA543%"); //GET Z Pins Buried Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double ZPinsBuried = StrVar.toDouble(&ok);
        if (ok) {
            m_stage.setPinsBuried(ZPinsBuried);
            Logger::logInfo("Z Pins Buried Position: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Z Pins Buried Position, last requestData: " + getLastCommand());
}
void AxesController::getZPinsExposed()
{
    sendCommand("$DA544%"); //GET Z Pins Exposed Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double ZPinsExposed = StrVar.toDouble(&ok);
        if (ok) {
            m_stage.setPinsExposed(ZPinsExposed);
            Logger::logInfo("Z Pins Exposed Position: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Z Pins Exposed Position, last requestData: " + getLastCommand());
}
void AxesController::getLoadX2Base()
{
    sendCommand("$DA512%"); //GET Load_X_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double X2Base = StrVar.toDouble(&ok);
        if (ok) {
            m_Xaxis.saveHomePos(X2Base);
            Logger::logInfo("X Load Position: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get X Load position, last requestData: " + getLastCommand());
}
void AxesController::getLoadY2Base()
{
    sendCommand("$DA522%"); //GET Load_Y_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Y2Base = StrVar.toDouble(&ok);
        if (ok) {
            m_Yaxis.saveHomePos(Y2Base);
            Logger::logInfo("Y Load Position: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get Y Load position, last requestData: " + getLastCommand());
}
void AxesController::getLoadZ2Base()
{
    sendCommand("$DA532%"); //GET Load_Z_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Z2Base = StrVar.toDouble(&ok);
        if (ok) {
            m_Zaxis.saveHomePos(Z2Base);
            Logger::logInfo("Z Load Position: " + StrVar + "");
        }
    }
    else
        Logger::logCritical("Could Not get Z Load position, last requestData: " + getLastCommand());
}

void AxesController::setAxisStateMachinesIdle()
{
    emit ISM_TransitionIdle(); // init state machine to idle
    emit HSM_TransitionIdle(); // home state machine to idle
    emit TSSM_TransitionIdle();// two spot state machine to idle
    emit ScanSM_TransitionIdle(); // scan state machine
    emit STSM_TransitionIdle(); // stage test
}


void AxesController::resetAxes()
{
    sendCommand("$A9%");
    readResponse();
}

void AxesController::togglePinsOn() // TODO: rename to pins up
{
    QString StrCmd{};
    QString pin_pos = QString::number(m_stage.getPinsExposedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    sendCommand(StrCmd);
    readResponse();
    Logger::logInfo("Stage Pins : enabled");
    emit pinsStateChanged(true);
}
void AxesController::togglePinsOff() // TODO: rename to pins down
{
    QString StrCmd{};
    QString pin_pos = QString::number(m_stage.getPinsBuriedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    sendCommand(StrCmd);
    readResponse();
    Logger::logInfo("Stage Pins : disabled");
    emit pinsStateChanged(false);
}

void AxesController::toggleJoystickOn()
{
    sendCommand("$BD%");
    readResponse();
}

void AxesController::toggleJoystickOff()
{
    sendCommand("$BF%");
    readResponse();
}

void AxesController::toggleN2PurgeOn()
{    
    sendCommand("$C701%"); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
    readResponse();
}
void AxesController::toggleN2PurgeOff()
{
    sendCommand("$C700%"); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
    readResponse();
}

void AxesController::toggleVacOn()
{
    sendCommand("$C801%"); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
    readResponse();
}

void AxesController::toggleVacOff()
{
    sendCommand("$C800%"); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
    readResponse();
}

