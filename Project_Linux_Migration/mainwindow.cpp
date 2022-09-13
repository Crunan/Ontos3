#include "mainwindow.h"
#include "qvalidator.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "logger.h"

#include <math.h>
#include <QRegularExpression>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <string>

using namespace std::literals;

struct TEMP {
public:
    double Value;
} ThermoCouple;

struct MFController {
public:
    int ActualFlowInteger;
    double ActualFlow;
    double RecipeFlow;
    double LoadedFlow;
    double Range;
    bool MFCLoadRecipeFlow;
} MFC[5];

struct PWR {
public:
    int RecipeSetPoint;
    int LoadedSetPoint;
    int ActualPForward;
    int ActualPReflected;
    bool LoadRecipePower;
    int MAXRF_PF_WATTS;
} RF;

struct TUNER_POS {
public:
    int LoadedSetPoint;
    double LoadedSetPointPct;
    double ActualPos;
    double ActualPosPct;
    bool LoadTunerPos;
} TUNER;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    status(new QLabel),
    settings(new SettingsDialog),
    serial(new QSerialPort(this))
{

    ui->setupUi(this);
    this->setWindowTitle("ATMOS INTERFACE");

    hideServiceFunctions();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    ui->statusBar->addWidget(status);

    timer->start(100ms);

    initActionsConnections();

    connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    connect(timer, &QTimer::timeout, this, &MainWindow::mainLoopTick);

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

}
MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Atmos Interface"),
                       tr("The <b>Atmos Interface</b> is the latest"
                          "modern GUI for Plasma applications."));
}
void MainWindow::hideServiceFunctions() const {
    ui->textCMDbox->setHidden(true);
    ui->CMDlabel->setHidden(true);
    ui->Send_CMD_button->setHidden(true);
    ui->Clear_Button->setHidden(true);
    ui->textRCVbox->setHidden(true);
    ui->RCVlabel->setHidden(true);
    ui->RF_Loaded_LCD->setHidden(true);
    ui->MB_Loaded_LCD->setHidden(true);
    ui->gas1_loaded_SLPM->setHidden(true);
    ui->gas2_loaded_SLPM->setHidden(true);
    ui->gas3_loaded_SLPM->setHidden(true);
    ui->gas4_loaded_SLPM->setHidden(true);
}
void MainWindow::showStatusMessage(const QString &message) {
    status->setText(message);
}
void MainWindow::initActionsConnections() {
    connect(ui->actionSave_Recipe, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(ui->actionLoad_Recipe, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
    connect(ui->actionDebug_Mode, &QAction::triggered, this, &MainWindow::debugMode);    
}
void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
void MainWindow::closeSerialPort() {
    SM_state = SHUTDOWN;
    if (serial->isOpen()) {
        resetCTL();
        serial->close();
    }

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    logDebug("Port Disconnected");
    showStatusMessage(tr("Disconnected"));
}
void MainWindow::openSerialPort() {
    const SettingsDialog::Settings p = settings->settings();    
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {        
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        resetCTL();        
        CTLResetTimeOut = 2500ms / timer->interval();
        (DEBUG_MODE) ? SM_state = IDLE : SM_state = STARTUP;
        Logger::init();
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        showStatusMessage(tr("Open Port error"));
    }
}
void MainWindow::resetCTL() {
    writeRequest("$90%", 4);
    readData();
}
void MainWindow::mainLoopTick() {
    static bool mainLoopRunning;
    if (mainLoopRunning)
        return;

    mainLoopRunning = true;

    stateMachine();

    mainLoopRunning = false;
}
void MainWindow::shutDownProgram() {
    closeSerialPort();
    Logger::clean();
    MainWindow::close();
}
void MainWindow::stateMachine() {
    switch(SM_state) {
      case STARTUP:
        if (CTLResetTimeOut > 0ms) {
            CTLResetTimeOut -= 1ms;
            break;
        }
        else {
            logDebug("Main State Machine Start Up");
            RunStartup();
            SM_state = POLLING;
            UpdateStatus(0);
            break;
        }
      case POLLING:
        RunCheckInput();
        SM_PollCounter += 1;
        if (SM_PollCounter >= SM_POLL_PERIOD) {
            SM_PollCounter = 0;
            RunPolling();
            UpdateStatus(StatusBits);
        }
        break;
      case IDLE:
        break;
      case SHUTDOWN:
        break;
        UpdateStatus(0);
    }
}
void MainWindow::RunCheckInput() {
    checkStepMBLeftInput();
    checkStepMBRightInput();
    checkMFC4LoadFlowInput();
    checkMFC3LoadFlowInput();
    checkMFC2LoadFlowInput();
    checkMFC1LoadFlowInput();
    checkRFLoadPowerInput();
    checkMBTunerPosInput();
    checkAutoModeInput();
    checkRunRecipeInput();
    checkSetDefaultRecipeInput();
}
void MainWindow::checkStepMBLeftInput() {
    if (Step_MB_Left) {
        writeRequest(MBLeftSpeed.toUtf8(), MBLeftSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
        readData();
        Step_MB_Left = false;
    }
}
void MainWindow::checkStepMBRightInput() {
    if (Step_MB_Right) {
        writeRequest(MBRightSpeed.toUtf8(), MBRightSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
        readData();
        Step_MB_Right = false;
    }
}
void MainWindow::checkMFC4LoadFlowInput() {
    float FltVar;
    QString StrVar;
    bool ok = false;
    if (MFC[4].MFCLoadRecipeFlow) {
        FltVar = ui->gas4_recipe_SLPM->text().toFloat(&ok);
        StrVar = "$4104" + ui->gas4_recipe_SLPM->text() + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        if (RCV.length() > 6) {
            MFC[4].LoadedFlow = FltVar;
            ui->gas4_loaded_SLPM->setText(ui->gas4_recipe_SLPM->text());
        }
        MFC[4].MFCLoadRecipeFlow = false;
    }
}
void MainWindow::checkMFC3LoadFlowInput() {
    float FltVar;
    QString StrVar;
    bool ok = false;
    if (MFC[3].MFCLoadRecipeFlow) {
        FltVar = ui->gas3_recipe_SLPM->text().toFloat(&ok);
        StrVar = "$4103" + ui->gas3_recipe_SLPM->text() + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        if (RCV.length() > 6) {
            MFC[3].LoadedFlow = FltVar;
            ui->gas3_loaded_SLPM->setText(ui->gas3_recipe_SLPM->text());
        }
        MFC[3].MFCLoadRecipeFlow = false;
    }
}
void MainWindow::checkMFC2LoadFlowInput() {
    float FltVar;
    QString StrVar;
    bool ok = false;
    if (MFC[2].MFCLoadRecipeFlow) {
        FltVar = ui->gas2_recipe_SLPM->text().toFloat(&ok);
        StrVar = "$4102" + ui->gas2_recipe_SLPM->text() + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        if (RCV.length() > 6) {
            MFC[2].LoadedFlow = FltVar;
            ui->gas2_loaded_SLPM->setText(ui->gas2_recipe_SLPM->text());
        }
        MFC[2].MFCLoadRecipeFlow = false;
    }
}
void MainWindow::checkMFC1LoadFlowInput() {
    float FltVar;
    QString StrVar;
    bool ok = false;
    if (MFC[1].MFCLoadRecipeFlow) {
        FltVar = ui->gas1_recipe_SLPM->text().toFloat(&ok);
        StrVar = "$4101" + ui->gas4_recipe_SLPM->text() + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        if (RCV.length() > 6) {
            MFC[1].LoadedFlow = FltVar;
            ui->gas1_loaded_SLPM->setText(ui->gas1_recipe_SLPM->text());
        }
        MFC[1].MFCLoadRecipeFlow = false;
    }
}
void MainWindow::checkRFLoadPowerInput() {
    int IntVal;
    QString StrVar;
    bool ok = false;
    if (RF.LoadRecipePower) {
        IntVal = ui->RF_Recipe_LCD->value();
        StrVar = DecIntToDecStr(IntVal, 4);
        StrVar = "$42" + StrVar + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_RF_WATTS  $42xxxx% xxxx = Watts; resp[!42xxxx#]
        readData();
        if (RCV.length() > 4) {
            StrVar = RCV.mid(3, 4);
            if (StringIsValidIntChars(StrVar)) {
                RF.LoadedSetPoint = StrVar.toInt(&ok);
                ui->RF_Loaded_LCD->display(RF.LoadedSetPoint);
            }
        RF.LoadRecipePower = false;
        }
    }
}
void MainWindow::checkMBTunerPosInput() {
    QString StrVar;
    bool ok = false;
    if (TUNER.LoadTunerPos) {
        StrVar = "$43" + StrVar.setNum(ui->MB_Recipe_LCD->value()) + "%";
        writeRequest(StrVar.toUtf8(), StrVar.length()); //SET_RCP_MS_POS  $43xxxx$ xxxx = Base10 MB Motor Pos; resp[!43xxxx#]
        readData();
        if (RCV.length() > 4) {
            StrVar = RCV.mid(3,4);
            if (StringIsValidDoubleChars(StrVar)) {
                TUNER.LoadedSetPointPct = StrVar.toDouble(&ok);
                ui->MB_Loaded_LCD->display(TUNER.LoadedSetPointPct);
            }
        TUNER.LoadTunerPos = false;
        }
    }
}
void MainWindow::checkAutoModeInput() {
    if (ToggleAutoMode) {
        AutoModeOn = !AutoModeOn;
        if (AutoModeOn) {
            writeRequest("$8601%", 6); //SET_AUTO_MAN 0x86 //$860p% p=1 AutoMode, p=0 ManualMode
            readData();
            ui->MB_Left_Button->setEnabled(false);
            ui->MB_Right_Button->setEnabled(false);
        }
        else {
            writeRequest("$8600%", 6); //SET_AUTO_MAN 0x86 //$860p% p=1 AutoMode, p=0 ManualMode
            readData();
            ui->MB_Left_Button->setEnabled(true);
            ui->MB_Right_Button->setEnabled(true);
        }

        ToggleAutoMode = false;
    }
}
void MainWindow::checkRunRecipeInput() {
    if (ToggleRunRecipe) {
        RunRecipeOn = !RunRecipeOn;
        if (RunRecipeOn) {
            writeRequest("$8701%", 6);
            readData();
        } else {
            writeRequest("$8700%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
            readData();
        }

        ToggleRunRecipe = false;
    }
}
void MainWindow::checkSetDefaultRecipeInput() {
    if (SetDefaultRecipe) {
        writeRequest("$44%", 4); //SET_DEF_RECIPE $44%; resp [!44#](RCV.length() < 50)
        readData();
        SetDefaultRecipe = false;
    }
}

void MainWindow::RunPolling() {
    getCTLStatus();
    CTLStatusChanged = didCTLStatusChange();
    splitTheRCVInformation();
    //! [0]
    StatusBitsStrToHex();
    UpdateStatus(StatusBits);
    //! [1]
    setTunerPosition();
    displayTunerPosition();
    //! [2]
    setRFPower();
    displayRFValue();
    //! [3]
    setReflectedPower();
    displayReflectedPower();
    //! [4]
    setExecuteRecipe();
    //! [5]
    setMFC1();
    displayMFC1();
    //! [6]
    setMFC2();
    displayMFC2();
    //! [7]
    setMFC3();
    displayMFC3();
    //! [8]
    setMFC4();
    displayMFC4();
    //! [9]
    setTempValue();
    displayTemperature();
    //! [10]
    HandShakeStatusBitsStrToHex();
    UpdateHandshakeStatus(HandshakeStatusBits);
}
void MainWindow::HandShakeStatusBitsStrToHex() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[10];
    StringIsValidHexChars(StrVar);
    HandshakeStatusBits = StrVar.toInt(&ok, 16);
}
void MainWindow::displayTemperature() {
    ui->temp_LCD->display(ThermoCouple.Value);
    ui->Temp_bar->setValue(ThermoCouple.Value);
}
void MainWindow::setTempValue() {
    QString StrVar;
    double DblVal;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[9];
    StringIsValidDoubleChars(StrVar);
    DblVal = StrVar.toDouble(&ok);
    ThermoCouple.Value = DblVal;
}
void MainWindow::displayMFC4() {
    ui->gas4_actual_SLPM->setText(QString().setNum(MFC[4].ActualFlow, 'F', 3));
    ui->gas4_progressbar->setValue(MFC[4].ActualFlowInteger);
}
void MainWindow::setMFC4() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[8];
    StringIsValidDoubleChars(StrVar);
    MFC[4].ActualFlow = StrVar.toDouble(&ok);
    MFC[4].ActualFlowInteger = StrVar.toInt(&ok);
}
void MainWindow::displayMFC3() {
    ui->gas3_actual_SLPM->setText(QString().setNum(MFC[3].ActualFlow, 'F', 3));
    ui->gas3_progessbar->setValue(MFC[3].ActualFlowInteger);
}
void MainWindow::setMFC3() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[7];
    StringIsValidDoubleChars(StrVar);
    MFC[3].ActualFlow = StrVar.toDouble(&ok);
    MFC[3].ActualFlowInteger = StrVar.toInt(&ok);
}
void MainWindow::displayMFC2() {
    ui->gas2_actual_SLPM->setText(QString().setNum(MFC[2].ActualFlow, 'F', 2));
    ui->gas2_progressbar->setValue(MFC[2].ActualFlowInteger);
}
void MainWindow::setMFC2() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[6];
    StringIsValidDoubleChars(StrVar);
    MFC[2].ActualFlow = StrVar.toDouble(&ok);
    MFC[2].ActualFlowInteger = StrVar.toInt(&ok);
}
void MainWindow::displayMFC1() {
    ui->gas1_actual_SLPM->setText(QString().setNum(MFC[1].ActualFlow, 'F', 2));
    ui->gas1_progressbar->setValue(MFC[1].ActualFlowInteger);

}
void MainWindow::setMFC1() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[5];
    StringIsValidDoubleChars(StrVar);
    MFC[1].ActualFlow = StrVar.toDouble(&ok);
    MFC[1].ActualFlowInteger = StrVar.toInt(&ok);
}
void MainWindow::setExecuteRecipe() {
    QString StrVar;
    int IntVal;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[4];
    StringIsValidHexChars(StrVar);
    IntVal = StrVar.toInt(&ok, 16);

    (RunRecipeOn == false && IntVal == 0) ? RunRecipeOn = true :
                                            RunRecipeOn = false;
}
void MainWindow::displayReflectedPower() {
    if (RunRecipeOn) {
        ui->RefRF_Actual_LCD->display(RF.ActualPReflected);
        ui->RefRF_bar->setValue(RF.ActualPReflected);
    }
}
void MainWindow::setReflectedPower() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[3];
    StringIsValidIntChars(StrVar);
    RF.ActualPReflected = StrVar.toInt(&ok, 10);
}
void MainWindow::displayRFValue() {
    if (RunRecipeOn) {
        ui->RF_Actual_LCD->display(RF.ActualPForward);
        ui->RF_bar->setValue(RF.ActualPForward);
    }
}
void MainWindow::setRFPower() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[2];
    StringIsValidIntChars(StrVar);
    RF.ActualPForward = StrVar.toInt(&ok, 10);
}
void MainWindow::displayTunerPosition() {
    //TODO
    //This can probably be hooked up to a signal or connection.
    ui->MB_Actual_LCD->display(TUNER.ActualPosPct);
}
void MainWindow::setTunerPosition() {
    QString StrVar;
    double DblVal;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[1];
    StringIsValidDoubleChars(StrVar);
    ui->MB_Pos_Bar->setValue(StrVar.toInt(&ok));
    DblVal = StrVar.toDouble(&ok);
    TUNER.ActualPos = DblVal;
    TUNER.ActualPosPct = DblVal;
    //TODO
    //set this up with a signal and slot?
//    if (!AutoModeOn) {
//       (TUNER.ActualPosPct > 98) ? ui->MB_Right_Button->setDisabled(true) : ui->MB_Right_Button->setDisabled(false);
//       (TUNER.ActualPosPct < 2) ? ui->MB_Left_Button->setDisabled(true) : ui->MB_Left_Button->setDisabled(false);
//    }
}
void MainWindow::splitTheRCVInformation() {
    CTL_ParametersAndValues = RCV.split(QLatin1Char(';'));
}
void MainWindow::StatusBitsStrToHex() {
    QString StrVar;
    bool ok = false;
    StrVar = CTL_ParametersAndValues[0];
    StrVar = StrVar.mid(3);
    StringIsValidHexChars(StrVar);
    StatusBits = StrVar.toInt(&ok, 16);
}
bool MainWindow::didCTLStatusChange() {
    return CTLPCBStatus != RCV ? true : false;
}
void MainWindow::getCTLStatus() {
    writeRequest("$91%", 4); //GET_STATUS    $91% ; resp[!91LLRR#] LL = left LEDS, RR = right LEDS
    readData();
    CTLPCBStatus = RCV;
}

void MainWindow::UpdateStatus(int myStatusBits) {
    if (myStatusBits != StatusBitsWas)
        logInfo("Status Bits Change from " + BinInt2String(StatusBitsWas) + " to " + BinInt2String(myStatusBits));

    StatusBitsWas = myStatusBits;

    (myStatusBits & 0x0100) > 0 ? ui->actionGAS_1->setChecked(true) : ui->actionGAS_1->setChecked(false);
    (myStatusBits & 0x0200) > 0 ? ui->actionGAS_2->setChecked(true) : ui->actionGAS_2->setChecked(false);
    (myStatusBits & 0x0400) > 0 ? ui->actionGAS_3->setChecked(true) : ui->actionGAS_3->setChecked(false);
    (myStatusBits & 0x0800) > 0 ? ui->actionGAS_4->setChecked(true) : ui->actionGAS_4->setChecked(false);

    (myStatusBits & 0x1000) > 0 ? ui->actionV5->setChecked(true) : ui->actionV5->setChecked(false);
    (myStatusBits & 0x2000) > 0 ? ui->actionV6->setChecked(true) : ui->actionV6->setChecked(false);
    (myStatusBits & 0x4000) > 0 ? ui->actionV7->setChecked(true) : ui->actionV7->setChecked(false);
    (myStatusBits & 0x8000) > 0 ? ui->actionRF_ENABLED->setChecked(true) : ui->actionRF_ENABLED->setChecked(false);

    (myStatusBits & 0x0001) > 0 ? ui->actionPLASMA_ON->setChecked(true) : ui->actionPLASMA_ON->setChecked(false);
    (myStatusBits & 0x0002) > 0 ? ui->actionTUNING->setChecked(true) : ui->actionTUNING->setChecked(false);
    (myStatusBits & 0x0004) > 0 ? ui->actionAUTO_MODE->setChecked(true) : ui->actionAUTO_MODE->setChecked(false);
    (myStatusBits & 0x0008) > 0 ? ui->actionEXECUTE_RECIPE->setChecked(true) : ui->actionEXECUTE_RECIPE->setChecked(false);

    (myStatusBits & 0x0010) > 0 ? ui->actionESTOP_ON->setChecked(true) : ui->actionESTOP_ON->setChecked(false);
    (myStatusBits & 0x0020) > 0 ? ui->actionDO_CMD->setChecked(true) : ui->actionDO_CMD->setChecked(false);
    (myStatusBits & 0x0040) > 0 ? ui->actionHE_SIG->setChecked(true) : ui->actionHE_SIG->setChecked(false);
    (myStatusBits & 0x0080) > 0 ? ui->actionPROCESS_ABORT->setChecked(true) : ui->actionPROCESS_ABORT->setChecked(false);

}
void MainWindow::UpdateHandshakeStatus(int myHandshakeStatusBits) {
    if (myHandshakeStatusBits != HandshakeStatusBitsWas) {
        logInfo("Handshake Status Bits Change from " + BinInt2String(HandshakeStatusBitsWas) + " to " + BinInt2String(myHandshakeStatusBits));

        HandshakeStatusBitsWas = myHandshakeStatusBits;

        if ((myHandshakeStatusBits & 0x0001) > 0)
            ui->actionP_RDY->toggle();
        if ((myHandshakeStatusBits & 0x0002) > 0)
            ui->actionP_AVAIL->toggle();
        if ((myHandshakeStatusBits & 0x0004) > 0)
            ui->actionRUN_P->toggle();
    }
}
void MainWindow::RunStartup() {
    howManyMFCs();
    getBatchLogging();
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
    turnOffExecRecipe();
    GetTemp();
}
void MainWindow::howManyMFCs() {
    writeRequest("$2A0002%", 8);
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(8,1); //GET Number of MFCs (1-4) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
        if (StringIsValidIntChars(StrVar)) {
            NumMFC = StrVar.toInt();
            logInfo("Number of MFC's: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not set # of MFCs, last requestData: " + requestData );

}
void MainWindow::getBatchLogging() {
    writeRequest("$2A0011%", 8); //GET BatchIDLogging $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(8, 1);
        if (StringIsValidIntChars(StrVar)) {
            bool ok = false;
            BatchLoggingOnOFF = (StrVar.toInt(&ok) == 1) ? true : false;
            logInfo("Batch Logging On/Off: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve Batch Logging, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMBPosition() {
    writeRequest("$2A606%", 7); //GET RECIPE MB Start Position () $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 4);
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            TUNER.LoadedSetPoint = (StrVar.toInt(&ok));
            StrVar = RCV.mid(7,7);
            TUNER.LoadedSetPointPct = (StrVar.toDouble(&ok));
            ui->MB_Recipe_LCD->display(TUNER.LoadedSetPointPct);
            ui->MB_Loaded_LCD->display(TUNER.LoadedSetPointPct);
            logInfo("Loaded MB Setpoint: " + StrVar + " %");

        }
    }
    else
        logCritical("Could Not retrieve MB tuner setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeRFPosition() {
    writeRequest("$2A605%", 7); //GET RECIPE RF PWR Setpoint (Watts) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 4);
        if (StringIsValidIntChars(StrVar)) {
            bool ok = false;
            RF.LoadedSetPoint = (StrVar.toInt(&ok));
            RF.RecipeSetPoint = (StrVar.toInt(&ok));
            ui->RF_Loaded_LCD->display(RF.LoadedSetPoint);
            ui->RF_Recipe_LCD->display(RF.RecipeSetPoint);
            logInfo("Loaded RF Setpoint: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve RF setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC4Flow() {
    writeRequest("$2A604%", 7); //GET RECIPE MFC4 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            MFC[4].LoadedFlow = (StrVar.toDouble(&ok));
            ui->gas4_recipe_SLPM->setText(QString().setNum(MFC[4].LoadedFlow, 'F', 3));
            ui->gas4_loaded_SLPM->setText(QString().setNum(MFC[4].LoadedFlow, 'F', 3));
            logInfo("Loaded MFC 4 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 4 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC3Flow() {
    writeRequest("$2A603%", 7); //GET RECIPE MFC3 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            MFC[3].LoadedFlow = (StrVar.toDouble(&ok));
            ui->gas3_recipe_SLPM->setText(QString().setNum(MFC[3].LoadedFlow, 'F', 3));
            ui->gas3_loaded_SLPM->setText(QString().setNum(MFC[3].LoadedFlow, 'F', 3));
            logInfo("Loaded MFC 3 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 3 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC2Flow() {
    writeRequest("$2A602%", 7); //GET RECIPE MFC2 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            MFC[2].LoadedFlow = (StrVar.toDouble(&ok));
            ui->gas2_recipe_SLPM->setText(QString().setNum(MFC[2].LoadedFlow, 'F', 2));
            ui->gas2_loaded_SLPM->setText(QString().setNum(MFC[2].LoadedFlow, 'F', 2));
            logInfo("Loaded MFC 2 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 2 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC1Flow() {
    writeRequest("$2A601%", 7); //GET RECIPE MFC1 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            MFC[1].LoadedFlow = (StrVar.toDouble(&ok));
            ui->gas1_recipe_SLPM->setText(QString().setNum(MFC[1].LoadedFlow, 'F', 2));
            ui->gas1_loaded_SLPM->setText(QString().setNum(MFC[1].LoadedFlow, 'F', 2));
            logInfo("Loaded MFC 1 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 1 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getMFC4Range() {
        writeRequest("$8504%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
        readData();
        if (RCV.length() > 6) {
            QString StrVar = RCV.mid(5, (RCV.length() - 6));
            if (StringIsValidDoubleChars(StrVar)) {
                bool ok = false;
                double DblVar = StrVar.toDouble(&ok);
                MFC[4].Range = DblVar;
                logInfo("Loaded MFC 4 Range: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve MFC 4 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC3Range() {
        writeRequest("$8503%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
        readData();
        if (RCV.length() > 6) {
            QString StrVar = RCV.mid(5, (RCV.length() - 6));
            if (StringIsValidDoubleChars(StrVar)) {
                bool ok = false;
                double DblVar = StrVar.toDouble(&ok);
                MFC[3].Range = DblVar;
                logInfo("Loaded MFC 3 Range: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve MFC 3 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC2Range() {
        writeRequest("$8502%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
        readData();
        if (RCV.length() > 6) {
            QString StrVar = RCV.mid(5, (RCV.length() - 6));
            if (StringIsValidDoubleChars(StrVar)) {
                bool ok = false;
                double DblVar = StrVar.toDouble(&ok);
                MFC[2].Range = DblVar;
                logInfo("Loaded MFC 2 Range: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve MFC 2 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC1Range() {
        writeRequest("$8501%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
        readData();
        if (RCV.length() > 6) {
            QString StrVar = RCV.mid(5, (RCV.length() - 6));
            if (StringIsValidDoubleChars(StrVar)) {
                bool ok = false;
                double DblVar = StrVar.toDouble(&ok);
                MFC[1].Range = DblVar;
                logInfo("Loaded MFC 1 Range: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + requestData );
}
void MainWindow::getMaxRFPowerForward() {
        writeRequest("$2A705%", 7); //Get Max RF power forward  $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
        readData();
        if (RCV.length() > 7) {
            QString StrVar = RCV.mid(7, 3);
            if (StringIsValidIntChars(StrVar)) {
                bool ok = false;
                RF.MAXRF_PF_WATTS = StrVar.toInt(&ok);
                logInfo("Loaded Max RF Forward: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + requestData );
}
void MainWindow::getAutoMan() {
        writeRequest("$89%", 4); //GET_AUTO_MAN   $89%; resp [!890p#] p=1 AutoMode, p=0 ManualMode
        readData();
        if (RCV.length() > 3) {
            QString StrVar = RCV.mid(3, 2);
            if (StringIsValidHexChars(StrVar)) {
                bool ok = false;
                if (StrVar.toInt(&ok) == 1) {
                    AutoModeOn = true;
                    ui->actionAUTO_MODE->setChecked(true);
                }
                else
                    AutoModeOn = false;

                logInfo("Loaded Auto/Manual Setting: " + StrVar);
            }
        }
        else
            logCritical("Could Not retrieve Auto/Manual setting, last requestData sent: " + requestData );
} //first 3 axis ctl they do not initiate (slaves)
    //Host PC (MASTER) - Gives requestData executed immedietely (status commands can be polled)
    //HOST to never wait more than few ms
void MainWindow::GetTemp() {
    writeRequest("$8C%", 4);
    readData();
    if (RCV.length() > 3) {
        QString StrVar = RCV.mid(3, 4);
        if (StringIsValidDoubleChars(StrVar)) {
            bool ok = false;
            double dblTemp = (StrVar.toDouble(&ok));
            ui->temp_LCD->display(dblTemp);
            int Temp = StrVar.toInt(&ok);
            ui->Temp_bar->setValue(Temp);
            }
            logInfo("Loaded current temperature: " + StrVar);
        }
    else
        logCritical("Could Not retrieve temperature, last requestData sent: " + requestData );
}

void MainWindow::turnOffExecRecipe() {
        writeRequest("$8700%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
        readData();
        logInfo("Execute Recipe turned off");
}
//READ/WRITE
void MainWindow::writeRequest(QByteArray const &CMD_Str, qint64 CMD_Len) {
        lastRequest = CMD_Str;
        RCV = "";
        ui->textCMDbox->appendPlainText(CMD_Str);        
        serial->write(CMD_Str, CMD_Len);       
}

void MainWindow::readData() {
    if (serial->waitForReadyRead(1000)) {
        RCV = serial->readAll();
        while (serial->waitForReadyRead(100)) {
            if (RCV.contains('#')) {
                return;
            } else
                RCV += serial->readAll();
        }
        logDebug(RCV);
        ui->textRCVbox->appendPlainText(RCV);
    }
}

//VALIDATORS & CONVERTERS
QString MainWindow::BinInt2String(int Bits16) {
    QString rtn;
    Bits16 = Bits16 | 0x10000; //for bit 16 high
    rtn = QString::number(Bits16,2);
    return rtn.mid(1);
}
bool MainWindow::StringIsValidIntChars(QString testString) const {
    QIntValidator intValidator;
    int necessaryVariable{0};
    if (intValidator.validate(testString, necessaryVariable) == QIntValidator::Acceptable)
        return true;
    else
        return false;
}
bool MainWindow::StringIsValidDoubleChars(QString testString) const {
    QDoubleValidator DblValidator;
    int necessaryVariable{0};
    return DblValidator.validate(testString, necessaryVariable);
}
bool MainWindow::StringIsValidHexChars(QString testString) const {
    QRegularExpression HexChars("0123456789ABCDEF");
    QRegularExpressionValidator HexValidator(HexChars);
    int necessaryVariable{0};
    return HexValidator.validate(testString, necessaryVariable);
}
QString MainWindow::DecIntToDecStr(int DecInt, int numChar) {
    QString DecStr;
    DecStr = DecStr.setNum(DecInt, 10);

    while (DecStr.length() < numChar)
        DecStr = "0" + DecStr;

    return DecStr;
}
// LOGGING
void MainWindow::logDebug(QString debugMsg) const {
    qDebug() << debugMsg;
}
void MainWindow::logInfo(QString infoMsg) const {
    qInfo() << infoMsg;
}
void MainWindow::logWarning(QString warningMsg) const {
    qWarning() << warningMsg;
}
void MainWindow::logCritical(QString criticalMsg) const {
    qCritical() << criticalMsg;
}
void MainWindow::logFatal() const {
    qFatal("Fatal Log!");
}

void MainWindow::on_actionService_triggered(){
    (ui->textCMDbox->isHidden()) ? ui->textCMDbox->setHidden(false) :
                                   ui->textCMDbox->setHidden(true);
    (ui->CMDlabel->isHidden()) ? ui->CMDlabel->setHidden(false) :
                                 ui->CMDlabel->setHidden(true);
    (ui->Send_CMD_button->isHidden()) ? ui->Send_CMD_button->setHidden(false) :
                                        ui->Send_CMD_button->setHidden(true) ;
    (ui->Clear_Button->isHidden()) ? ui->Clear_Button->setHidden(false) :
                                    ui->Clear_Button->setHidden(true) ;
    (ui->textRCVbox->isHidden()) ? ui->textRCVbox->setHidden(false) :
                                   ui->textRCVbox->setHidden(true);
    (ui->RCVlabel->isHidden()) ? ui->RCVlabel->setHidden(false) :
                                 ui->RCVlabel->setHidden(true);
}


void MainWindow::on_actionMFCLoadedValues_triggered() {
    (ui->RF_Loaded_LCD->isHidden()) ? ui->RF_Loaded_LCD->setHidden(false) : ui->RF_Loaded_LCD->setHidden(true);
    (ui->MB_Loaded_LCD->isHidden()) ? ui->MB_Loaded_LCD->setHidden(false) : ui->MB_Loaded_LCD->setHidden(true);

    (ui->gas1_loaded_SLPM->isHidden()) ? ui->gas1_loaded_SLPM->setHidden(false) : ui->gas1_loaded_SLPM->setHidden(true);
    (ui->gas2_loaded_SLPM->isHidden()) ? ui->gas2_loaded_SLPM->setHidden(false) : ui->gas2_loaded_SLPM->setHidden(true);
    (ui->gas3_loaded_SLPM->isHidden()) ? ui->gas3_loaded_SLPM->setHidden(false) : ui->gas3_loaded_SLPM->setHidden(true);
    (ui->gas4_loaded_SLPM->isHidden()) ? ui->gas4_loaded_SLPM->setHidden(false) : ui->gas4_loaded_SLPM->setHidden(true);
}

void MainWindow::on_MB_Left_Button_clicked() {
    Step_MB_Left = true;
}

void MainWindow::on_MB_Right_Button_clicked() {
    Step_MB_Right = true;
}


void MainWindow::on_autoMode_Button_clicked() {
    ToggleAutoMode = true;
}


void MainWindow::on_plsmaBtn_clicked()
{
    ToggleRunRecipe = true;
}


void MainWindow::on_Heater_Button_clicked() {
    ToggleHeater = true;
}

void MainWindow::on_Send_CMD_button_clicked() {
    QByteArray StrVar = ui->textCMDbox->toPlainText().toLocal8Bit();
    ui->textCMDbox->clear();
    writeRequest(StrVar, StrVar.length());
    StrVar.clear();
}


void MainWindow::on_Clear_Button_clicked() {
    ui->textCMDbox->clear();
    ui->textRCVbox->clear();
}


void MainWindow::debugMode() {
    DEBUG_MODE = !DEBUG_MODE;
}

void MainWindow::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Current Recipe"), "",
        tr("Recipe List (*.rcp);;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;

            }

            QString RecipeString = "<MFC1>" + ui->gas1_recipe_SLPM->text() + '\n' +
                                   "<MFC2>" + ui->gas2_recipe_SLPM->text() + '\n' +
                                   "<MFC3>" + ui->gas3_recipe_SLPM->text() + '\n' +
                                   "<MFC4>" + ui->gas4_recipe_SLPM->text() + '\n' +
                                   "<PWR>" + QString().setNum(ui->RF_Recipe_LCD->value()) + '\n' +
                                   "<TUNER>" + QString().setNum(ui->MB_Recipe_LCD->value()) + '\n' +
                                   "<AUTOSCAN>" + QString().setNum(AutoModeOn);

            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_4_5);
            logInfo("Saved " + fileName + " : " + RecipeString);
            out << RecipeString;
        }
}
void MainWindow::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Recipe from File"), "",
        tr("Recipe File (*.rcp);;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);
            QString RecipeData;

            in >> RecipeData;
            QStringList RecipeParam = RecipeData.split(QLatin1Char('<'));
            loadGUI(RecipeParam);
            showStatusMessage(tr("Loaded Recipe"));

    }
}

void MainWindow::loadGUI(QStringList RecipeParam) {
    QStringList RecipeParamValue;
    QStringList Params;
    QStringList Values;
    bool ok = false;
    int valueIndex;
    for (int i = 1; i < RecipeParam.size(); i++) {
            RecipeParamValue = RecipeParam[i].split(QLatin1Char('>'));
            Params << RecipeParamValue[0];
            Values << RecipeParamValue[1];
        }
    if (Params.contains("MFC1")) {
        valueIndex = Params.indexOf("MFC1");
        MFC[1].RecipeFlow = Values[valueIndex].toDouble(&ok);
        ui->gas1_recipe_SLPM->setText(QString().setNum(MFC[1].RecipeFlow, 'F', 2));
    }
    if (Params.contains("MFC2")) {
        valueIndex = Params.indexOf("MFC2");
        MFC[2].RecipeFlow = Values[valueIndex].toDouble(&ok);
        ui->gas2_recipe_SLPM->setText(QString().setNum(MFC[2].RecipeFlow, 'F', 2));
    }
    if (Params.contains("MFC3")) {
        valueIndex = Params.indexOf("MFC3");
        MFC[3].RecipeFlow = Values[valueIndex].toDouble(&ok);
        ui->gas3_recipe_SLPM->setText(QString().setNum(MFC[3].RecipeFlow, 'F', 3));
    }
    if (Params.contains("MFC4")) {
        valueIndex = Params.indexOf("MFC4");
        MFC[4].RecipeFlow = Values[valueIndex].toDouble(&ok);
        ui->gas4_recipe_SLPM->setText(QString().setNum(MFC[4].RecipeFlow, 'F', 3));
    }
    if (Params.contains("PWR")) {
        valueIndex = Params.indexOf("PWR");
        RF.RecipeSetPoint = Values[valueIndex].toInt(&ok);
        ui->RF_Recipe_LCD->display(RF.RecipeSetPoint);
    }
    if (Params.contains("TUNER")) {
        valueIndex = Params.indexOf("TUNER");
        TUNER.LoadedSetPoint = Values[valueIndex].toInt(&ok);
        ui->MB_Recipe_LCD->display(TUNER.LoadedSetPoint);
    }
}

void MainWindow::on_gas1_setpoint_button_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 1 Setpoint","SLPM: ", 0, 0, 20.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ui->gas1_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 2));
        ui->gas1_sliderbar->setValue(doubVal * 100);
        MFC[1].MFCLoadRecipeFlow = true;
    }
}


void MainWindow::on_gas2_setpoint_button_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 2 Setpoint","SLPM: ", 0, 0, 20.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ui->gas2_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 2));
        ui->gas2_sliderbar->setValue(doubVal * 100);
        MFC[2].MFCLoadRecipeFlow = true;
    }
}


void MainWindow::on_gas3_setpoint_button_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 3 Setpoint","SLPM: ", 0, 0, 0.300, 3, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ui->gas3_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 3));
        ui->gas3_sliderbar->setValue(doubVal * 100);
        MFC[3].MFCLoadRecipeFlow = true;
    }
}


void MainWindow::on_gas4_setpoint_button_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 4 Setpoint","SLPM: ", 0, 0, 0.300, 3, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ui->gas4_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 3));
        ui->gas4_sliderbar->setValue(doubVal * 100);
        MFC[4].MFCLoadRecipeFlow = true;
    }
}


void MainWindow::on_RF_Setpoint_Button_clicked(){
    bool ok;
    int intVal = QInputDialog::getInt(this, "RF Power Setpoint","Watts: ", 0, 0, 200, 1, &ok,Qt::WindowFlags());
    if (ok) {
        ui->RF_Recipe_LCD->display(intVal);
        RF.LoadRecipePower = true;
    }
}


void MainWindow::on_MB_Setpoint_Button_clicked(){
        bool ok;
        double doubVal = QInputDialog::getDouble(this, "Matchbox Setpoint","%: ", 50, 0, 100, 2, &ok,Qt::WindowFlags(), 0);
        if (ok) {
            ui->MB_Recipe_LCD->display(doubVal);
            TUNER.LoadTunerPos = true;
        }
}


void MainWindow::on_mb_Dial_valueChanged(int value)
{
    ui->MB_Recipe_LCD->display(value);
    TUNER.LoadTunerPos = true;
}


void MainWindow::on_RF_Dial_valueChanged(int value)
{
    ui->RF_Recipe_LCD->display(value);
    RF.LoadRecipePower = true;
}



