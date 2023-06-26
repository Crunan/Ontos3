#include "include/plasmacontroller/plasmacontroller.h"

PlasmaController::PlasmaController(SerialComms& serialComm, QWidget* parent)
  : QObject(parent),
    plasmaHead(),
    pwr(),
    tuner(),
    mfcs({ new MFC(1), new MFC(2), new MFC(3), new MFC(4) }),
    commandMap(),
    config_(),
    axisCTL_(nullptr),
    serialComm_(serialComm)
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

}

void PlasmaController::setCommandMap(const QMap<QString, QPair<QString, QString>>& map)
{
    commandMap.setCommandMap(map);
}
QString PlasmaController::findCommandValue(QString command) const
{
    return commandMap.findCommandValue(command);
}

QString PlasmaController::prepareCommand(QString cmd, const QString& setpoint)
{
    // Remove the trailing '%' character
    cmd.chop(1);

    // Add the setpoint to the command
    cmd += setpoint;

    // Add the '%' character back to the command
    cmd += "%";

    return cmd;
}

void PlasmaController::sendSerialCommand(const QString& data)
{
    serialComm_.setOutgoingData(data);
    serialComm_.writeOutgoingData();
}

// MFC
void PlasmaController::handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow)
{
    QString setpoint = QString::number(recipeFlow);
    QString command = "$820" + QString::number(mfcNumber) + "%";
    command = prepareCommand(command, setpoint);
    sendSerialCommand(command);
}

void PlasmaController::handleSetMFCDefaultRecipeCommand(const int mfcNumber, const double recipeFlow)
{
    QString setpoint = QString::number(recipeFlow);
    QString command = "$2A60" + QString::number(mfcNumber) + "%";
    command = prepareCommand(command, setpoint);
    sendSerialCommand(command);
}

void PlasmaController::handleSetMFCRangeCommand(const int mfcNumber, const double range)
{
    QString setpoint = QString::number(range);
    QString command = "$2A" + QString::number(mfcNumber) + "02%";
    command = prepareCommand(command, setpoint);
    sendSerialCommand(command);
}

// TUNER
void PlasmaController::handleSetTunerRecipePositionCommand(const double recipePosition)
{
    QString command = "$43" + QString::number(recipePosition) + "%";
    sendSerialCommand(command);
}

void PlasmaController::handleSetTunerDefaultRecipeCommand(const double defaultPosition)
{
    QString command = "$2A606" + QString::number(defaultPosition) + "%";
    sendSerialCommand(command);
}

void PlasmaController::handleSetTunerAutoTuneCommand(const bool value)
{
    QString command = "$860" + QString::number(value) + "%";
    sendSerialCommand(command);
}


//PlasmaController::getPlasmaHead() {
//    //serial->setOutgoingData(command.getCommandString(""));
//}
//void PlasmaController::RunPolling() {
//    getCTLStatus();
//    //didCTLStatusChange(); //is this for logging?
//    splitRCV();
//    //! [0]
//    setStatusBitsFromPoll();
//    UpdateStatus();
//    //! [1]
//    setTunerPosition();
//    displayTunerPosition();
//    //! [2]
//    setRFPower();
//    displayRFValue();
//    //! [3]
//    setReflectedPower();
//    displayReflectedPower();
//    //! [4]
//    //! setExecRecipe()
//    //! [5]
//    setMFC1();
//    MFC1ActualFlow();
//    //! [6]
//    setMFC2();
//    MFC2ActualFlow();
//    //! [7]
//    setMFC3();
//    MFC3ActualFlow();
//    //! [8]
//    setMFC4();
//    MFC4ActualFlow();
//    //! [9]
//    setTempValue();
//    getHeadTemp();
//    //! [10]
//    //UpdateHandshakeStatus();
//    //! [11]
//    getAxisStatus();
//}

//void MainWindow::setMFC4() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[8])) {
//        MFC[4].setActualFlow(CTLparametersAndValues[8]);
//    };
//}
//void MainWindow::setMFC3() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[7])) {
//        MFC[3].setActualFlow(CTLparametersAndValues[7]);
//    };
//}
//void MainWindow::setMFC2() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[6])) {
//        MFC[2].setActualFlow(CTLparametersAndValues[6]);
//    };
//}
//void MainWindow::setMFC1() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[5])) {
//        MFC[1].setActualFlow(CTLparametersAndValues[5]);
//    };
//}
//void MainWindow::setReflectedPower() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[3])) {
//        RF.setActualRefWatts(CTLparametersAndValues[3]);
//    };
//}
//void MainWindow::setRFPower() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[2])) {
//        RF.setActualWatts(CTLparametersAndValues[2]);
//    };
//}
//void MainWindow::setTunerPosition() {
//    if (StringIsValidDoubleChars(CTLparametersAndValues[1])) {
//        TUNER.setActualPosition(CTLparametersAndValues[1]);
//    };
//}
//void MainWindow::setTempValue() {
//    plasmahead.setHeadTemp(CTLparametersAndValues[9]);
//}
//void MainWindow::setStatusBitsFromPoll() {
//    bool ok;
//    QString StrVar = CTLparametersAndValues[0].mid(3);
//    StatusBits = StrVar.toInt(&ok, 16);
//}

//void MainWindow::didStatusBitsChange() {
//    if (StatusBits != StatusBitsWas)
//        logInfo("Status Bits Change from " + QString::number(StatusBitsWas) + " to " + QString::number(StatusBits));
//}

//void MainWindow::splitRCV() {
//    PCBStatus = RCV;
//    CTLparametersAndValues = RCV.split(QLatin1Char(';'));
//}

//void MainWindow::RunStartup() {
//    GetExeCfg();
//    CTLStartup();
//    AxisStartup();
//}



//void MainWindow::CTLStartup() {
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
//}


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
