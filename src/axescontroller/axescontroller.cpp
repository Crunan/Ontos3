#include "include/axescontroller/axescontroller.h"
#include <QSerialPort>
#include "settingsdialog.h"
#include <QString>
#include "logger.h"
#include <QTimer>
#include "recipe.h"

const int AUX_INPUT_BUFFER_MAX_SIZE = 90;
const int SERIAL_RESPONSE_TIMEOUT = 2000; // timeout waiting for control pcb response (milliseconds)

const QString XAXIS_QSTR = "0";
const QString YAXIS_QSTR = "1";
const QString ZAXIS_QSTR = "2";

AxesController::AxesController(QObject *parent) :
    QObject(parent),
    m_ledStatus(),
    m_serialInterface(),
    m_Xaxis(this),
    m_Yaxis(this),
    m_Zaxis(this),
    m_stage(),
    m_axisStatus(),
    m_sameStateXYZ(false),
    m_joystickOn(false)
{
    SetupInitAxesStateMachine();
    SetupHomeAxesStateMachine();
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
}

//////////////// State Machine Setup
void AxesController::SetupInitAxesStateMachine()
{
    m_pInitAxesIdleState = new QState();
    m_pInitAxesStartupState = new QState();
    m_pInitAxesWaitForDoneState = new QState();
    m_pInitAxesInitializedState = new QState();

    // construct transitions
    m_pInitAxesIdleState->addTransition(this, SIGNAL(ISM_TransitionStartup()), m_pInitAxesStartupState);
    m_pInitAxesStartupState->addTransition(this, SIGNAL(ISM_TransitionWaitForDone()), m_pInitAxesWaitForDoneState);
    m_pInitAxesWaitForDoneState->addTransition(this, SIGNAL(ISM_TransitionInitialized()), m_pInitAxesInitializedState);
    m_pInitAxesInitializedState->addTransition(this, SIGNAL(ISM_TransitionIdle()), m_pInitAxesIdleState);
    m_pInitAxesWaitForDoneState->addTransition(this, SIGNAL(ISM_TransitionIdle()), m_pInitAxesIdleState);
    m_pInitAxesStartupState->addTransition(this, SIGNAL(ISM_TransitionIdle()), m_pInitAxesIdleState);

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
    m_homeStateMachine.addState(m_pHomeAxesStartupState);
    m_homeStateMachine.addState(m_pHomeAxesWaitParkZState);
    m_homeStateMachine.addState(m_pHomeAxesHomeXYState);
    m_homeStateMachine.addState(m_pHomeAxesWaitHomeXYState);
    m_homeStateMachine.addState(m_pHomeAxesHomeZState);
    m_homeStateMachine.addState(m_pHomeAxesWaitHomeZState);

    // add transitions to and from the super state
    m_pHomeAxisSuperState->addTransition(this, SIGNAL(HSM_TransitionIdle()), m_pHomeAxesIdleState);
    m_pHomeAxisSuperState->addTransition(this, SIGNAL(HSM_TransitionShutdown()), m_pHomeAxesShutdownState);
    m_pHomeAxesIdleState->addTransition(this, SIGNAL(HSM_TransitionStartup()), m_pHomeAxisSuperState);

    // add the rest of the transitions
    m_pHomeAxesStartupState->addTransition(this, SIGNAL(HSM_TransitionStartupToWaitParkZ()), m_pHomeAxesWaitParkZState);
    m_pHomeAxesWaitParkZState->addTransition(this, SIGNAL(HSM_TransitionWaitParkZToHomeXY()), m_pHomeAxesHomeXYState);
    m_pHomeAxesHomeXYState ->addTransition(this, SIGNAL(HSM_TransitionHomeXYToWaitHomeXY()), m_pHomeAxesWaitHomeXYState);
    m_pHomeAxesWaitHomeXYState->addTransition(this, SIGNAL(HSM_TransitionHomeXYToHomeZ()), m_pHomeAxesHomeZState);
    m_pHomeAxesHomeZState->addTransition(this, SIGNAL(HSM_TransitionHomeZToWaitHomeZ()), m_pHomeAxesWaitHomeZState);
    m_pHomeAxesShutdownState->addTransition(this, SIGNAL(HSM_TransitionIdle()), m_pHomeAxesShutdownState);
    m_pHomeAxesIdleState->addTransition(this, SIGNAL(HSM_TransitionShutdown()), m_pHomeAxesShutdownState);

    // set initial state
    m_homeStateMachine.setInitialState(m_pHomeAxesIdleState);

    // start the state machine
    m_homeStateMachine.start();
}

//////////////// Serial Port
bool AxesController::open(const SettingsDialog& settings)
{
    const SettingsDialog::Settings p = settings.settings();
    m_serialInterface.setPortName(p.name);
    m_serialInterface.setBaudRate(p.baudRate);
    m_serialInterface.setDataBits(p.dataBits);
    m_serialInterface.setParity(p.parity);
    m_serialInterface.setStopBits(p.stopBits);
    m_serialInterface.setFlowControl(p.flowControl);
    if (!m_serialInterface.open(QIODevice::ReadWrite)) {
        // Failed to open the serial port
        return false;
    }
    // Signals connections
    //connect(&serialPort_, &QSerialPort::readyRead, this, &AxesController::readData);
    emit stagePortOpened();

    return true;
}

void AxesController::close()
{
    m_serialInterface.close();
}

bool AxesController::sendCommand(const QString& command)
{
    return m_serialInterface.sendCommand(command);
}

// read a single response
QString AxesController::readResponse()
{
    return m_serialInterface.readResponse();
}

QString AxesController::getPortErrorString()
{
    return m_serialInterface.errorString();
}

bool AxesController::isOpen()
{
    return m_serialInterface.isOpen();
}

void AxesController::StartInit()
{
    // assume we are in the idle state (possibly enforce this through enabling the button or not)
    emit ISM_TransitionStartup();
}


void AxesController::StartHome()
{
    // assume we are in the idle state (possibly enforce this through enabling the button or not)
    emit HSM_TransitionStartup();

}
//            RunScanBtn.Visible = False
//            SetTwoSpotBtn.Visible = False
//            SetDiameterBtn.Visible = False
//            HomeAxesBtn.Text = "STOP"

//            PinsSquare.BackColor = Color.Gainsboro
//            b_HasPins = False 'This is so the first time the button is hit, the button will bury the pins
//////////////// State Machines
void AxesController::RunInitAxesSM()
{
    if (m_initStateMachine.configuration().contains(m_pInitAxesStartupState)) { // in Startup state
        //SM set to idle
        //TwoSpotSM.setState(TSSM_IDLE); // TODO: need to implement
        //ScanSM.setState(SASM_IDLE); // TODO: need to implement
        //ScanSM.setSubState(SSSM_IDLE); // TODO: need to implement

        //send commands
        stopMotors();
        setValve2("0");
        sendInitCMD();

        //GUI status
        //ui->Stagepins_button->setChecked(true); // TODO: need to implement

        // update UI
        emit stageStatusUpdate("Initializing Axes", "");

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

        // update UI
        emit stageStatusUpdate("Stage Initialized", "");

        // initiate transtion to Idle state
        emit ISM_TransitionIdle();
    }
}

void AxesController::RunHomeAxesSM()
{
    //Start by parking Z in a safe position to move
    if (m_homeStateMachine.configuration().contains(m_pHomeAxesStartupState)) { // in Startup state

        if (nextStateReady()) {

//            RunScanBtn.Visible = False TODO: needs implementing
//            SetTwoSpotBtn.Visible = False
//            SetDiameterBtn.Visible = False
//            HomeAxesBtn.Text = "STOP"

//            PinsSquare.BackColor = Color.Gainsboro
//            b_HasPins = False 'This is so the first time the button is hit, the button will bury the pins

            // put init axis state machine in the Idle state
            emit ISM_TransitionIdle();

            // update UI//            RunScanBtn.Visible = False
            //            SetTwoSpotBtn.Visible = False
            //            SetDiameterBtn.Visible = False
            //            HomeAxesBtn.Text = "STOP"

            //            PinsSquare.BackColor = Color.Gainsboro
            //            b_HasPins = False 'This is so the first time the button is hit, the button will bury the pins
            emit setHomeButtonText("STOP");
            emit stageStatusUpdate("Homing Startup - Parking Z", "Homing X & Y");

            move(ZAXIS_QSTR, m_Zaxis.getMaxSpeedQStr(), m_stage.getPinsBuriedPosQStr());
            Logger::logInfo("Homing Start.  Speed = " + m_Zaxis.getMaxSpeedQStr());

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

        move(XAXIS_QSTR, m_Xaxis.getMaxSpeedQStr(), m_Xaxis.getHomePosQStr());
        move(YAXIS_QSTR, m_Yaxis.getMaxSpeedQStr(), m_Yaxis.getHomePosQStr());

        QString sMsg("Homing X & Y."
                     " X speed = " + m_Xaxis.getMaxSpeedQStr() +
                     " X pos = " + m_Xaxis.getHomePosQStr() +
                     " Y speed = " + m_Yaxis.getMaxSpeedQStr() +
                     " Y pos = " + m_Yaxis.getHomePosQStr());

        Logger::logInfo(sMsg);

        // transition to wait home XY
        emit HSM_TransitionHomeXYToWaitHomeXY();
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesWaitHomeXYState)) { // in wait home xy state

        if (nextStateReady()) {

            // transition to home Z
            emit HSM_TransitionHomeXYToHomeZ();

            Logger::logInfo("X & Y Homed");

            //        RunScanBtn.Visible = True // TODO: needs implementing
            //        If b_ENG_mode Then
            //            SetTwoSpotBtn.Visible = True
            //            SetDiameterBtn.Visible = True
            //        End If
        }
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesHomeZState)) { // in home z state

        // update the UI
        emit stageStatusUpdate("Homing Z", "");

        Logger::logInfo("Homing Z");

        move(ZAXIS_QSTR, m_Zaxis.getMaxSpeedQStr(), m_stage.getPinsBuriedPosQStr());

        // transition to wait home Z
        emit HSM_TransitionHomeZToWaitHomeZ();
    }
    else if (m_homeStateMachine.configuration().contains(m_pHomeAxesWaitHomeZState)) { // in wait home z state

        // transition to wait home Z
        emit HSM_TransitionHomeZToWaitHomeZ();

        if (nextStateReady()) {

            // update the UI
            emit stageStatusUpdate("", "");
            emit setHomeButtonText("LOAD");

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

        emit setHomeButtonText("LOAD");

        move(ZAXIS_QSTR, m_Zaxis.getMaxSpeedQStr(), m_stage.getPinsBuriedPosQStr());

        emit stageStatusUpdate("", "");

        // transition to idle
        emit HSM_TransitionIdle();

//        RunScanBtn.Visible = True // TODO: needs implementing
//        If b_ENG_mode Then
//            SetTwoSpotBtn.Visible = True
//            SetDiameterBtn.Visible = True
//        End If
    }
}

void AxesController::RunTwoSpotAxesSM()
{
    if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotStartupState)) {

        toggleJoystickOn();

        // setTwoSpotBtnText("STOP"); // TODO: needs implementing
        emit stageStatusUpdate("Use Controller for Stage", "Spot First Point");

        Logger::logInfo("TwoSpotSM Start Up");

        emit TSSM_TransitionGetFirst();
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotGetFirstState)) {

        if (m_joystickOn) {

            emit stageStatusUpdate("Use Controller for Stage", "Release JoyStick Button");

            // translate into ph coordinates and save
            m_Xaxis.setTwoSpotFirstPoint(TranslateCoordXPH2Base(m_Xaxis.getPosition()));
            m_Yaxis.setTwoSpotFirstPoint(TranslateCoordYPH2Base(m_Yaxis.getPosition()));

            Logger::logInfo("TwoSpotSM Got First");

            emit TSSM_TransitionJoyBtnOff();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotWaitJoyBtnOffState)) {

        if (!m_joystickOn) {

            Logger::logInfo("TwoSpotSM Getting Second");

            emit stageStatusUpdate("Spot Second Point", "");

            emit TSSM_TransitionGetSecond();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotGetSecondState)) {

        if (m_joystickOn) {

            // translate to ph coordinates and save
            m_Xaxis.setTwoSpotSecondPoint(TranslateCoordXPH2Base(m_Xaxis.getPosition()));
            m_Yaxis.setTwoSpotSecondPoint(TranslateCoordYPH2Base(m_Yaxis.getPosition()));

            //determine box orientation and corners for scanning
            m_Xaxis.checkAndSetDimensions();
            m_Yaxis.checkAndSetDimensions();

            // updateTwoSpotXYText(); TODO: Needs implementing

            Logger::logInfo("TwoSpotSM Got Second - done");

            emit TSSM_TransitionShutdown();
        }
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotShutdownState)) {

        toggleJoystickOff();

        emit TSSM_TransitionIdle();

        emit stageStatusUpdate("", "");

        //setTwoSpotBtnText("TWO SPOT"); TODO: needs implementing
    }
    else if (m_twoSpotStateMachine.configuration().contains(m_pTwoSpotIdleState)) {
        // no op
    }
}


void AxesController::move(QString axis, QString speed, QString position)
{
    setSpeed(axis, speed);
    setAbsMove(axis, position);
}

void AxesController::setSpeed(QString axis, QString speed)
{
    QString command;
    command = "$B40" + axis + speed + "%";
    sendCommand(command);
    QString response = readResponse();

    // update UI
    emit stageResponseReceived(response);

    Logger::logInfo("Move " + axis + " Speed: " + speed + " /sec ");
}
void AxesController::setAbsMove(QString axis, QString position)
{
    QString command;
    command = "$B60" + axis + position + "%";
    sendCommand(command);
    QString response = readResponse();

    // update UI
    emit stageResponseReceived(response);

    Logger::logInfo("Move " + axis + " to: " + position);
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

void AxesController::setValve2(QString toggle)
{
    sendCommand("$C70" + toggle + "%"); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
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

//////////////// Axis
void AxesController::AxisStartup()
{
    getXMaxSpeed();
    getYMaxSpeed();
    getZMaxSpeed();
    getXp2Base();
    getYp2Base();
    getZp2Base();
    getXs2PH();
    getYs2PH();
    getPHSlitLength();
    getPHSlitWidth();
    getPHSafetyZGap();
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
    parseStatus(response);
}


void AxesController::setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble)
{
    bool ok;
    int LEDstates = firstHexStrNibble.toInt(&ok, 16); //First byte
    LEDstates = LEDstates<<8;
    LEDstates += secondHexStrNibble.toInt(&ok, 16); //First byte
    m_ledStatus.setStatusBits(LEDstates);
}

void AxesController::updateAxisStatus()
{
    if (m_axisStatus.length() == 11) {
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
        //update GUI positions
        //updateAxisPosition();
        //set doors
    //    setDoors(); // TODO: Need to implement
    //    setJoyBtnOn(); // TODO: Need to implement
    //    setVacOn(); // TODO: Need to implement
    //    setN2PurgeOn(); // TODO: Need to implement
        //log any change
        //    if (didStatusChange()) {
        //        logAxesStatus();
        //    }
    }

    emit
}

void AxesController::setXMaxPos(const double maxPos)
{
    m_Xaxis.setMaxPos(maxPos);
}

void AxesController::setXMaxSpeed(const double maxSpeed)
{
    m_Xaxis.setMaxSpeed(maxSpeed);
}

void AxesController::setXHomePos(const double homePos)
{
    m_Xaxis.setHomePos(homePos);
}

void AxesController::setYMaxPos(const double maxPos)
{
    m_Yaxis.setMaxPos(maxPos);
}

void AxesController::setYMaxSpeed(const double maxSpeed)
{
    m_Yaxis.setMaxSpeed(maxSpeed);
}

void AxesController::setYHomePos(const double homePos)
{
   m_Yaxis.setHomePos(homePos);
}

void AxesController::setZMaxPos(const double maxPos)
{
    m_Zaxis.setMaxPos(maxPos);
}

void AxesController::setZMaxSpeed(const double maxSpeed)
{
    m_Zaxis.setMaxSpeed(maxSpeed);
}

void AxesController::setZHomePos(const double homePos)
{
    m_Zaxis.setHomePos(homePos);
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
            setXMaxSpeed(maxSpeed);
            Logger::logInfo("X Max Speed: " + StrVar + "");
        }
    }
    else
        //Logger::logCritical("Could Not get max speed for X, last requestData: " + requestData );
        Logger::logCritical("Could Not get max speed for X");
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
            setYMaxSpeed(maxSpeed);
            Logger::logInfo("Y Max Speed: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get max speed for Y, last requestData: " + requestData );
        Logger::logCritical("Could Not get max speed for Y");
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
            setZMaxSpeed(maxSpeed);
            Logger::logInfo("Z Max Speed: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get max speed for Z, last requestData: " + requestData );
        Logger::logCritical("Could Not get max speed for Z");
}
void AxesController::getXp2Base()
{
    sendCommand("$DA510%"); //GET Xp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Xp2Base = StrVar.toDouble(&ok);
        if (ok) {
            //CoordParam.setXp2Base(StrVar); // TODO: need to implement
            Logger::logInfo("Xp to Base: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get X relative to Base, last requestData: " + requestData );
        Logger::logCritical("Could Not get X relative to Base");
}
void AxesController::getYp2Base()
{
    sendCommand("$DA520%"); //GET Yp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Yp2Base = StrVar.toDouble(&ok);
        if (ok) {
            //CoordParam.setYp2Base(StrVar); // TODO: need to implement
            Logger::logInfo("Yp to Base: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get Y relative to Base, last requestData: " + requestData );
        Logger::logCritical("Could Not get Y relative to Base");
}
void AxesController::getZp2Base()
{
    sendCommand("$DA530%"); //GET Zp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Zp2Base = StrVar.toDouble(&ok);
        if (ok) {
            //CoordParam.setZp2Base(StrVar); // TODO: need to implement
            Logger::logInfo("Zp to Base: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get Z relative to Base, last requestData: " + requestData);
        Logger::logCritical("Could Not get Z relative to Base");
}
void AxesController::getXs2PH()
{
    sendCommand("$DA511%"); //GET Xs_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Xs2PH = StrVar.toDouble(&ok);
        if (ok) {
            //CoordParam.setlaserX2Base(StrVar); // TODO: need to implement
            Logger::logInfo("Xs to Plasma Head: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get X relative to Plasma head, last requestData: " + requestData );
        Logger::logCritical("Could Not get X relative to Plasma head");
}
void AxesController::getYs2PH()
{
    sendCommand("$DA521%"); //GET Ys_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double Ys2PH = StrVar.toDouble(&ok);
        if (ok) {
            //CoordParam.setlaserY2Base(StrVar); // TODO: need to implement
            Logger::logInfo("Ys to Plasma Head: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get Y relative to Plasma head, last requestData: " + requestData );
        Logger::logCritical("Could Not get Y relative to Plasma head");
}
void AxesController::getPHSlitLength()
{
    sendCommand("$DA540%"); //GET Plasma Head Slit Length (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSlitLength = StrVar.toDouble(&ok);
        if (ok) {
            //plasmahead.setPlasmaHeadSlitLength(StrVar); // TODO: need to implement
            Logger::logInfo("Plasma Head Slit Length: " + StrVar + " (mm)");
        }
    }
    else
        //logCritical("Could Not get Plasma head slit length, last requestData: " + requestData );
        Logger::logCritical("Could Not get Plasma head slit length");
}
void AxesController::getPHSlitWidth()
{
    sendCommand("$DA541%"); //GET Plasma Head Slit Width (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSlitWidth = StrVar.toDouble(&ok);
        if (ok) {
            //plasmahead.setPlasmaHeadSlitWidth(StrVar); // TODO: need to implement
            Logger::logInfo("Plasma Head Slit Width: " + StrVar + " (mm)");
        }
    }
    else
        //logCritical("Could Not get Plasma head slit width, last requestData: " + requestData );
        Logger::logCritical("Could Not get Plasma head slit width");
}
void AxesController::getPHSafetyZGap()
{
    sendCommand("$DA542%"); //GET Plasma Head Safety Gap $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    QString response = readResponse();
    if (response.length() > 7) {
        QString StrVar = response.mid(7, (response.length() - 8));
        bool ok = false;
        double PHSafetyZGap = StrVar.toDouble(&ok);
        if (ok) {
            //plasmahead.setSafetyGap(StrVar); // TODO: need to implement
            Logger::logInfo("Plasma Head Z Safety Gap: " + StrVar + " (mm)");
        }
    }
    else
        //logCritical("Could Not get Plasma Head Z Safety Gap, last requestData: " + requestData );
        Logger::logCritical("Could Not get Plasma Head Z Safety Gap");
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
            m_stage.setPinsBuried(StrVar); // TODO: need to implement
            Logger::logInfo("Z Pins Buried Position: " + StrVar + " (mm)");
        }
    }
    else
        //logCritical("Could Not get Z Pins Buried Position, last requestData: " + requestData );
        Logger::logCritical("Could Not get Z Pins Buried Position");
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
            m_stage.setPinsExposed(StrVar); // TODO: need to implement
            Logger::logInfo("Z Pins Exposed Position: " + StrVar + " (mm)");
        }
    }
    else
        Logger::logCritical("Could Not get Z Pins Exposed Position");
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
            m_Xaxis.setHomePos(X2Base);
            Logger::logInfo("X Load Position: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get X Load position, last requestData: " + requestData );
        Logger::logCritical("Could Not get X Load position");
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
            m_Yaxis.setHomePos(Y2Base);
            Logger::logInfo("Y Load Position: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get Y Load position, last requestData: " + requestData );
        Logger::logCritical("Could Not get Y Load position");
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
            m_Zaxis.setHomePos(Z2Base);
            Logger::logInfo("Z Load Position: " + StrVar + "");
        }
    }
    else
        //logCritical("Could Not get Z Load position, last requestData: " + requestData );
        Logger::logCritical("Could Not get Z Load position");
}

void AxesController::setAxisStateMachinesIdle()
{
    emit ISM_TransitionIdle(); // init state machine to idle
    emit HSM_TransitionIdle(); // home state machine to idle

    //TwoSpotSM.setState(TSSM_IDLE); // TODO: need to implement
    //ScanSM.setState(SASM_IDLE); // TODO: need to implement
    //ScanSM.setSubState(SSSM_IDLE); // TODO: need to implement
}

void AxesController::resetAxes()
{
    sendCommand("$A9%");
    QString response = readResponse();
}

void AxesController::togglePinsOn()
{
    QString StrCmd{};
    QString pin_pos = QString::number(m_stage.getPinsExposedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    sendCommand(StrCmd);
    QString response = readResponse();
    Logger::logInfo("Stage Pins : enabled");
}
void AxesController::togglePinsOff()
{
    QString StrCmd{};
    QString pin_pos = QString::number(m_stage.getPinsBuriedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    sendCommand(StrCmd);
    QString response = readResponse();
    Logger::logInfo("Stage Pins : disabled");
}

void AxesController::toggleJoystickOn()
{
    sendCommand("$BE%");
    readResponse();
    Logger::logInfo("Joystick : enabled");
    m_joystickOn = true;
}

void AxesController::toggleJoystickOff()
{
    sendCommand("$BF%");
    readResponse();
    Logger::logInfo("Joystick : disabled");
    m_joystickOn = false;
}

void AxesController::toggleN2PurgeOn(Recipe &recipe)
{
    recipe.setPurge(true);
    Logger::logInfo("Recipe N2 purge : enabled");
}
void AxesController::toggleN2PurgeOff(Recipe &recipe)
{
    recipe.setPurge(false);
    Logger::logInfo("Recipe N2 purge : disabled");
}

