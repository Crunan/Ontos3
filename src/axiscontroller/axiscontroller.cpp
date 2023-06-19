#include "include/axiscontroller/axiscontroller.h"

//Controller::Controller(QObject* parent) : QObject(parent) {
//    // Initialize member variables
//    currentStatus = "";
//    ledStates = 0;
//    doorsOpen = false;
//    joystickOn = false;
//    vacuumOn = false;
//    nitrogenPurgeOn = false;
//}

//Controller::~Controller() {
//    // Destructor
//}

//QString Controller::getCurrentStatus() const {
//    return currentStatus;
//}

//void Controller::setCurrentStatus(const QString &status) {
//    if (currentStatus != status) {
//        currentStatus = status;
//        emit currentStatusChanged();
//    }
//}

//QStringList Controller::getAxisStatus() const {
//    return axisStatus;
//}

//void Controller::setAxisStatus(const QStringList &statusList) {
//    if (axisStatus != statusList) {
//        axisStatus = statusList;
//        emit axisStatusChanged();
//    }
//}

//int Controller::getLedStates() const {
//    return ledStates;
//}

//void Controller::setLedStates(int states) {
//    if (ledStates != states) {
//        ledStates = states;
//        emit ledStatesChanged();
//    }
//}

//bool Controller::areDoorsOpen() const {
//    return doorsOpen;
//}

//void Controller::setDoorsOpen(bool isOpen) {
//    if (doorsOpen != isOpen) {
//        doorsOpen = isOpen;
//        emit doorsOpenChanged();
//    }
//}

//bool Controller::isJoystickOn() const {
//    return joystickOn;
//}

//void Controller::setJoystickOn(bool isOn) {
//    if (joystickOn != isOn) {
//        joystickOn = isOn;
//        emit joystickOnChanged();
//    }
//}

//bool Controller::isVacuumOn() const {
//    return vacuumOn;
//}

//void Controller::setVacuumOn(bool isOn) {
//    if (vacuumOn != isOn) {
//        vacuumOn = isOn;
//        emit vacuumOnChanged();
//    }
//}

//bool Controller::isNitrogenPurgeOn() const {
//    return nitrogenPurgeOn;
//}

//void Controller::setNitrogenPurgeOn(bool isOn) {
//    if (nitrogenPurgeOn != isOn) {
//        nitrogenPurgeOn = isOn;
//        emit nitrogenPurgeOnChanged();
//    }
//}

//bool Controller::sameStateXYZ() {
//    return false;
//}

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
////    //X
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
