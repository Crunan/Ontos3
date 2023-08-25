#include "include/axescontroller/axiscontroller.h"

AxisController::AxisController(QObject* parent) : QObject(parent) {
    // Initialize member variables
    currentStatus = "";
    ledStates = 0;
    doorsOpen = false;
    joystickOn = false;
    vacuumOn = false;
    nitrogenPurgeOn = false;
}

AxisController::~AxisController() {
    // Destructor
}

QString AxisController::getCurrentStatus() const {
    return currentStatus;
}

void AxisController::setCurrentStatus(const QString &status) {
    if (currentStatus != status) {
        currentStatus = status;
        emit currentStatusChanged();
    }
}

QStringList AxisController::getAxisStatus() const {
    return axisStatus;
}

void AxisController::setAxisStatus(const QStringList &statusList) {
    if (axisStatus != statusList) {
        axisStatus = statusList;
        emit axisStatusChanged();
    }
}

int AxisController::getLedStates() const {
    return ledStates;
}

void AxisController::setLedStates(int states) {
    if (ledStates != states) {
        ledStates = states;
        emit ledStatesChanged();
    }
}

bool AxisController::areDoorsOpen() const {
    return doorsOpen;
}

void AxisController::setDoorsOpen(bool isOpen) {
    if (doorsOpen != isOpen) {
        doorsOpen = isOpen;
        emit doorsOpenChanged();
    }
}

bool AxisController::isJoystickOn() const {
    return joystickOn;
}

void AxisController::setJoystickOn(bool isOn) {
    if (joystickOn != isOn) {
        joystickOn = isOn;
        emit joystickOnChanged();
    }
}

bool AxisController::isVacuumOn() const {
    return vacuumOn;
}

void AxisController::setVacuumOn(bool isOn) {
    if (vacuumOn != isOn) {
        vacuumOn = isOn;
        emit vacuumOnChanged();
    }
}

bool AxisController::isNitrogenPurgeOn() const {
    return nitrogenPurgeOn;
}

void AxisController::setNitrogenPurgeOn(bool isOn) {
    if (nitrogenPurgeOn != isOn) {
        nitrogenPurgeOn = isOn;
        emit nitrogenPurgeOnChanged();
    }
}

bool AxisController::sameStateXYZ() {
    return false;
}

////void Controller::AxisStartup() {
////    command.getXMaxSpeed();
////    command.getYMaxSpeed();
////    command.getZMaxSpeed();
////    command.getPlasmaHeadXCoordinate();
////    command.getPlasmaHeadYCoordinate();
////    command.getPlasmaHeadZCoordinate();
////    command.getPlasmaLaserXCoordinate();
////    command.getPlasmaLaserYCoordinate();
////    command.getPHSlitLength();
////    command.getPHSlitWidth();
////    command.getPHSafetyZGap();
////    command.getZPinsBuried();
////    command.getZPinsExposed();
////    command.getLoadX2Base();
////    command.getLoadY2Base();
////    command.getLoadZ2Base();
////    command.setAxisStateMachinesIdle();
////}
////void Controller::parseStatus() {
////    QString statusToParse = RCV;
////    statusToParse = statusToParse.mid(3); //remove the first 3 chars
////    statusToParse = statusToParse.mid(0, statusToParse.length() -1); //remove # from response before split
////    axisStatus = statusToParse.split(QLatin1Char(';')); //split string up
////    updateAxisStatus();
////}

////void Controller::updateAxisStatus() {
////    //handle LED status
////    setLEDstate(axisStatus[0], axisStatus[1]);
////    //X//void Axis::RunInitAxisSM()
//{
//    Select Case SMInitAxes.State

//        Case IASM_START_UP
//            WriteCommand("$B3%", 4) 'Stop All Motors
//            ResponseLen = ReadResponse(0)
//            'turn off Substrate N2 Purge (assume it's on)
//            WriteCommand("$C700%", 6) 'SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
//            ResponseLen = ReadResponse(0)

//            SMScan.State = SCSM_IDLE 'disable other motor moving state machines and buttons
//            SMScan.SubState = SCSM_SUB_IDLE
//            StageTestSM.SetState(STSM_IDLE)
//            SMHomeAxes.State = HASM_IDLE
//            SMTwoSpot.State = TSSM_IDLE
//            RunScanBtn.Visible = False
//            SetTwoSpotBtn.Visible = False
//            SetDiameterBtn.Visible = False
//            HomeAxesBtn.Visible = False

//            WriteLogLine("Initializing Axes")
//            CurrentStepTxtBox.Text = "Initializing Axes"
//            NextStepTxtBox.Text = ""

//            WriteCommand("$B500%", 6) 'Start initialize X axis
//            ResponseLen = ReadResponse(0)
//            WriteCommand("$B501%", 6) 'Start initialize Y axis
//            ResponseLen = ReadResponse(0)
//            WriteCommand("$B502%", 6) 'Start initialize Z axis
//            ResponseLen = ReadResponse(0)


//            SMInitAxes.State = IASM_WAIT_FOR_DONE

//        Case IASM_WAIT_FOR_DONE
//            If AxesStatus.b_XYZSameState = True And AxesStatus.XState >= ASM_IDLE Then
//                SMInitAxes.State = IASM_INITIALIZED
//                WriteLogLine("Stage Initialized")

//                CurrentStepTxtBox.Text = "Stage Initialized"

//                RunScanBtn.Text = "START SCAN"
//                SetTwoSpotBtn.Text = "SET TWO SPOT"
//                SetDiameterBtn.Text = "SET DIAMETER"
//                HomeAxesBtn.Text = "LOAD POSITION"
//                RunScanBtn.Visible = True
//                HomeAxesBtn.Visible = True
//                Vacbtn.Visible = True
//                RecipeButtonPins.Visible = True
//                AutoVacSquare.Visible = True
//                PinsSquare.Visible = True
//                PinsSquare.BackColor = Color.Lime
//                b_HasPins = True 'This is so the first time the button is hit, the button will bury the pins
//                If b_ENG_mode Then
//                    SetTwoSpotBtn.Visible = True//void Axis::RunInitAxisSM()
//{
//                    SetDiameterBtn.Visible = True
//                End If


//            End If

//        Case IASM_IDLE
//            'do nothing
//        Case IASM_INITIALIZED
//            'do nothing
//    End Select
//}
////    X.setState(axisStatus[2]);
////    X.setError(axisStatus[3]);
////    X.setPosition(axisStatus[4]);
////    //Y
////    Y.setState(axisStatus[5]);
////    Y.setError(axisStatus[6]);
////    Y.setPosition(axisStatus[7]);
////    //Z
////    Z.setState(axisStatus[8]);
////    Z.setError(axisStatus[9]);
////    Z.setPosition(axisStatus[10]);
////    //XYZ same? (probably stopped)
////    setSameStateXYZsame();
////    //update GUI positions
////    updateAxisPosition();
////    //set doors
////    setDoors();
////    setJoyBtnOn();
////    setVacOn();
////    setN2PurgeOn();
////    //log any change
////    //    if (didStatusChange()) {
////    //        logAxesStatus();
////    //    }
////}
//////Axis Commands
////void MainWindow::setDoors() {
////    doorsOpen = isBitSet(LEDstates, 6);
////}
////void MainWindow::setJoyBtnOn() {
////    joystickOn = isBitSet(LEDstates, 14);
////}
////void MainWindow::setVacOn() {
////    vacuumOn = isBitSet(LEDstates, 12);
////}
////void MainWindow::setN2PurgeOn() {
////    nitrogenPurgeOn = isBitSet(LEDstates, 11);
////}
////bool MainWindow::isJoyStickOn() {
////    return joystickOn;
////}
////void MainWindow::setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble) {
////    bool ok;
////    LEDstates = firstHexStrNibble.toInt(&ok, 16); //First byte
////    LEDstates = LEDstates<<8;
////    LEDstates += secondHexStrNibble.toInt(&ok, 16); //First byte
////}
