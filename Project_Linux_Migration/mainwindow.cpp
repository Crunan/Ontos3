#include "mainwindow.h"
#include "qvalidator.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "logger.h"
#include "coordinate.h"

#include <math.h>
#include <QRegularExpression>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <string>

using namespace std::literals;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    status(new QLabel),
    settings(new SettingsDialog),
    serial(new QSerialPort(this))
{

    ui->setupUi(this);

    this->setWindowTitle("ONTOS3 INTERFACE");

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    ui->statusBar->addWidget(status);

    ui->tabWidget->setCurrentIndex(0);
    timer->start(100ms);

    initActionsConnections();

    connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    connect(timer, &QTimer::timeout, this, &MainWindow::mainLoopTick);

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

}
MainWindow::~MainWindow() {
    delete settings;
    delete ui;
}


//struct configuration {
//    QString ExeConfigPath = "./config/";
//    QString ExeConfigPathFileName = "default";
//    QString MFC1;
//    QString MFC2;
//    QString MFC3;
//    QString MFC4;

//    public:
//    QString getMFC1() const;
//    QString getMFC2() const;
//    QString getMFC3() const;
//    QString getMFC4() const;

//    void setMFC1(const QString &newMFC1);
//    void setMFC2(const QString &newMFC2);
//    void setMFC3(const QString &newMFC3);
//    void setMFC4(const QString &newMFC4);
//    QString getExeConfigPath() const;
//    QString getExeConfigPathFileName() const;

//} config;

//struct MFController {
//public:
//    double m_actualFlow;
//    double m_loadedFlow;
//    double m_range;
//    bool m_readyToLoad;

//    public:

//    void setReadyToLoad(bool toggle) {
//        m_readyToLoad = toggle;
//    }
//    bool getReadyToLoad() {
//        return m_readyToLoad;
//    }
//    void setLoadedFlow(QString flow) {
//        bool ok;
//        m_loadedFlow = flow.toDouble(&ok);
//        this->setReadyToLoad(true);
//    }
//    void setRange(QString range) {
//        bool ok;
//        m_range = range.toDouble(&ok);
//    }
//    void setActualFlow(QString flow) {
//        bool ok;
//        m_actualFlow = flow.toDouble(&ok);
//    }
//    QString getLoadedFlowQStr() {
//        return QString::number(m_loadedFlow, 'f', 2);
//    }
//    QString getRange() {
//        return QString::number(m_range, 'f', 2);
//    }
//    QString getActualFlow() {
//        return QString::number(m_actualFlow, 'f', 2);
//    }
//    int getActualFlowInt() {
//        return int(m_actualFlow);
//    }
//    double getLoadedFlow() {
//        return m_loadedFlow;
//    }

//} MFC[5];

//struct PWR {
//    public:
//    int m_loadedSetPoint;
//    int m_actualPForward;
//    int m_actualPReflected;
//    int m_maxWatts;
//    bool m_readyToLoad;


//    public:
//    void setReadyToLoad(bool toggle) {
//        m_readyToLoad = toggle;
//    }
//    bool getReadyToLoad() {
//        return m_readyToLoad;
//    }
//    void setLoadedSetPoint(QString value) {
//        bool ok;
//        m_loadedSetPoint = value.toInt(&ok);
//        this->setReadyToLoad(true);
//    }
//    void setMaxWatts(QString max) {
//        bool ok;
//        m_maxWatts = max.toInt(&ok);
//    }
//    void setActualWatts(QString watts) {
//        bool ok;
//        m_actualPForward = watts.toInt(&ok, 10);
//    }
//    void setActualRefWatts(QString watts) {
//        bool ok;
//        m_actualPReflected = watts.toInt(&ok, 10);
//    }
//    int getLoadedSetPoint() {
//        return m_loadedSetPoint;
//    }
//    QString getLoadedSetPointQStr() {
//        return QString::number(m_loadedSetPoint);
//    }
//    int getActualWatts() {
//        return m_actualPForward;
//    }
//    int getReflectedWatts() {
//        return m_actualPReflected;
//    }
//} RF;

//struct TUNER_POS {
//    public:
//    double m_loadedSP;
//    double m_actualPos;
//    double m_actualPosPct;
//    bool m_autoTune;
//    bool m_autoTuneCMDFlag;
//    bool m_readyToLoad;

//    public:
//    void setReadyToLoad(bool toggle) {
//        m_readyToLoad = toggle;
//    }
//    bool getReadyToLoad() {
//        return m_readyToLoad;
//    }
//    void setAutoTuneCMDFlag(bool toggle) {
//        m_autoTuneCMDFlag = toggle;
//    }
//    void setLoadedValue(QString value) {
//        bool ok;
//        m_loadedSP = value.toDouble(&ok);
//        this->setReadyToLoad(true);
//    }
//    void setAutoMode(QString value) {
//        bool ok;
//        m_autoTune = value.toInt(&ok);
//        this->setAutoTuneCMDFlag(true);
//    }
//    void setActualPosition(QString pos) {
//        bool ok;
//        m_actualPos = pos.toDouble(&ok);
//    }

//    double getLoadedSP() {
//        return m_loadedSP;
//    }
//    int getLoadedSPInteger() {
//        return int(m_loadedSP);
//    }
//    bool getAutoTune() {
//        return m_autoTune;
//    }
//    double getActualPosition() {
//        return m_actualPos;
//    }
//    QString getLoadedSPQStr() {
//        return QString::number(m_loadedSP);
//    }
//    QString getAutoTuneQStr() {
//        return QString::number(m_autoTune);
//    }
//    bool getAutoTuneCMDFlag() {
//        return m_autoTuneCMDFlag;
//    }
//} TUNER;


////Init Axes SM
//enum InitStates {
//    IASM_IDLE,
//    IASM_STARTUP,
//    IASM_WAIT_FOR_DONE,
//    IASM_INITIALIZED
//};
////Home Axes SM
//enum HomeAxesStates {
//    HASM_STARTUP,
//    HASM_WAIT_PARK_Z,
//    HASM_HOME_XY,
//    HASM_WAIT_HOME_XY,
//    HASM_HOME_Z,
//    HASM_WAIT_HOME_Z,
//    HASM_SHUTDOWN,
//    HASM_IDLE
//};
////Two Spot SM
//enum TwoSpotAxesStates {
//    TSSM_SHUTDOWN,
//    TSSM_IDLE,
//    TSSM_STARTUP,
//    TSSM_GET_FIRST,
//    TSSM_WAIT_JOY_BTN_OFF,
//    TSSM_GET_SECOND
//};


//struct PARAMETERS {
//    int m_numMFCs;
//    bool m_batchLogging;
//    double m_maxXPos;
//    double m_maxXSpeed;
//    double m_homeXPos;
//    double m_maxYPos;
//    double m_maxYSpeed;
//    double m_homeYPos;
//    double m_maxZPos;
//    double m_maxZSpeed;
//    double m_homeZPos;

//    public:
//    void setMFCs(QString num) {
//        bool ok;
//        m_numMFCs = num.toInt(&ok);
//    }
//    void setBatchLogging(QString toggle) {
//        bool ok;
//        m_batchLogging = toggle.toInt(&ok);
//    }
//    void setXMaxPos(QString x) {
//        bool ok;
//        m_maxXPos = x.toDouble((&ok));
//    }
//    void setYMaxPos(QString y) {
//        bool ok;
//        m_maxYPos = y.toDouble((&ok));
//    }
//    void setZMaxPos(QString z) {
//        bool ok;
//        m_maxZPos = z.toDouble((&ok));
//    }

//    void setXHomePos(QString x) {
//        bool ok;
//        m_homeXPos = x.toDouble((&ok));
//    }
//    void setYHomePos(QString y) {
//        bool ok;
//        m_homeYPos = y.toDouble((&ok));
//    }
//    void setZHomePos(QString z) {
//        bool ok;
//        m_homeZPos = z.toDouble((&ok));
//    }

//    void setXMaxSpeed(QString speed) {
//        bool ok;
//        m_maxXSpeed = speed.toDouble((&ok));
//    }
//    void setYMaxSpeed(QString speed) {
//        bool ok;
//        m_maxYSpeed = speed.toDouble((&ok));
//    }
//    void setZMaxSpeed(QString speed) {
//        bool ok;
//        m_maxZSpeed = speed.toDouble((&ok));
//    }
//    double getXMaxPos() {
//        return m_maxXPos;
//    }
//    double getYMaxPos() {
//        return m_maxYPos;
//    }
//    double getZMaxPos() {
//        return m_maxZPos;
//    }

//    double getXHomePos() {
//        return m_homeXPos;
//    }
//    double getYHomePos() {
//        return m_homeYPos;
//    }
//    double getZHomePos() {
//        return m_homeZPos;
//    }
//    double getXMaxSpeed() {
//        return m_maxXSpeed;
//    }
//    double getYMaxSpeed() {
//        return m_maxYSpeed;
//    }
//    double getZMaxSpeed() {
//        return m_maxZSpeed;
//    }
//    QString getXMaxSpeedQStr() {
//        return QString::number(m_maxXSpeed, 'f', 2);
//    }
//    QString getYMaxSpeedQStr() {
//        return QString::number(m_maxYSpeed, 'f', 2);
//    }
//    QString getZMaxSpeedQStr() {
//        return QString::number(m_maxZSpeed, 'f', 2);
//    }
//    QString getXHomePosQStr() {
//        return QString::number(m_homeXPos);
//    }
//    QString getYHomePosQStr() {
//        return QString::number(m_homeYPos);
//    }
//    QString getZHomePosQStr() {
//        return QString::number(m_homeZPos);
//    }
//} Param;










////Init Axes SM
//struct INITAXESSTATEMACHINE {
//    private:
//    InitStates m_state;
//    public:
//    InitStates getState() {
//        return m_state;
//    }
//    void setState(InitStates state) {
//        m_state = state;
//    }
//} InitSM;

//struct HOMEAXESSTATEMACHINE {
//    private:
//    HomeAxesStates m_state;
//    public:
//    HomeAxesStates getState() {
//        return m_state;
//    }
//    void setState(HomeAxesStates state) {
//        m_state = state;
//    }
//} HomeSM;




//    ScannerState state;
//    ScannerSubState subState;
//    double minX, maxX, minY, maxY, minZ, maxZ;

//    double m_rowXwidth;
//    double m_colYlength;
//    double m_startXpos;
//    double m_startYpos;
//    double m_endYpos;
//    double m_scanYspeed = 1;
//    int m_numXrows;
//    double m_Xremaining;
//    int m_thisXrow;
//    int m_thisCycleNum;




//    void setZMinPos(double z) {
//        m_minZpos = z;
//    }
//    void setZMaxPos() {
//        bool ok;
//        m_maxZpos = CoordParam.getZp2Base() - recipe.getThickness().toDouble(&ok) - recipe.getGap().toDouble(&ok);
//    }
//    void setXRowWidth(double val1, double val2) {
//        m_rowXwidth = val1 - val2;
//    }
//    void setXTotal() {
//        m_Xremaining = this->getXmax() - this->getXmin();
//    }
//    void setXRemaining() {
//        m_Xremaining = m_Xremaining - this->getRowWidth();
//    }
//    void setThisCycle(int thisCycle) {
//        m_thisCycleNum = thisCycle;
//    }
//    void setThisXRow(int row) {
//        m_thisXrow = row;
//    }
//    void setNumRows(int val) {
//        m_numXrows = val;
//    }
//    void setXNewStartPos(double pos1, double pos2) {
//        m_startXpos = pos1 - pos2;
//    }
//    void setYStartPos() {
//        m_startYpos = this->getYmax() + plasmahead.getPlasmaHeadSlitWidth();
//    }
//    void setYEndPos() {
//        m_endYpos = this->getYmin() + plasmahead.getPlasmaHeadSlitWidth();
//    }
//    void setYSpeed() {
//        bool ok;
//        m_scanYspeed = recipe.getSpeed().toDouble(&ok);
//    }
//    void incrementNumRows() {
//        this->m_numXrows = this->getNumXrows() + 1;
//    }
//    void incrementThisRow() {
//        this->m_thisXrow = this->getThisXRow() + 1;
//    }
//    void incrementCycles() {
//        m_thisCycleNum += 1;
//    }


//    void setXSinglePass() {
//        m_startXpos = (this->getXmax() + this->getXmin()) / 2;
//    }
//    void setXMutltiplePass() {
//        m_startXpos = this->getXmax() - (this->getRowWidth() / 2);
//    }
//    void setSingleOrMultipleScan() {
//        if (this->getNumXrows() == 1) {
//            this->setXSinglePass();
//        }
//        else {
//            this->setXMutltiplePass();
//        }
//    }

//    ScannerState getState() {
//        return m_state;
//    }
//    ScannerSubState getSubState() {
//        return m_subState;
//    }
//    QString getXminQStr() {
//        return QString::number(m_minXpos);
//    }
//    QString getXmaxQStr() {
//        return QString::number(m_maxXpos);
//    }
//    QString getYminQStr() {
//        return QString::number(m_minYpos);
//    }
//    QString getYmaxQStr() {
//        return QString::number(m_maxYpos);
//    }
//    QString getXRemainingQStr() {
//        return QString::number(m_Xremaining);
//    }
//    QString getXNumRowsQStr() {
//        return QString::number(m_numXrows);
//    }
//    QString getXRowWidthQStr() {
//        return QString::number(m_rowXwidth);
//    }
//    QString getXThisRowQStr() {
//        return QString::number(m_thisXrow);
//    }
//    QString getXStartPosQStr() {
//        return QString::number(m_startXpos);
//    }
//    QString getYStartPosQStr() {
//        return QString::number(m_startYpos);
//    }
//    QString getYEndPosQStr() {
//        return QString::number(m_endYpos);
//    }
//    QString getScanYSpeedQStr() {
//        return QString::number(m_scanYspeed);
//    }
//    QString getThisCyclesQStr() {
//        return QString::number(m_thisCycleNum);
//    }
//    QString getZminQStr() {
//        return QString::number(m_minZpos);
//    }
//    QString getZmaxQStr() {
//        return QString::number(m_maxZpos);
//    }
//    double getXStartPos() {
//        return m_startXpos;
//    }
//    double getXRowWidth() {
//        return m_rowXwidth;
//    }
//    int getThisCycle() {
//        return m_thisCycleNum;
//    }

//    double getXmin() {
//        return m_minXpos;
//    }
//    double getXmax() {
//        return m_maxXpos;
//    }
//    double getYmin() {
//        return m_minYpos;
//    }
//    double getYmax() {
//        return m_maxYpos;
//    }
//    double getMinPerPH() {
//        return m_minPerPH;
//    }
//    double getMaxPerPH() {
//        return m_maxPerPH;
//    }
//    double getXremaining() {
//        return m_Xremaining;
//    }
//    double getNumXrows() {
//        return m_numXrows;
//    }
//    double getRowWidth() {
//        return m_rowXwidth;
//    }
//    int getThisXRow() {
//        return m_thisXrow;
//    }
//    bool finishedScanning() {
//        if (this->getThisXRow() > this->getNumXrows()) {
//            return true;
//        }
//        else {
//            return false;
//        }
//    }
//    bool keepScanning() {
//        if (this->getThisCycle() >= recipe.getCyclesInt()) {
//            return true;
//        }
//        else {
//            return false;
//        }
//    }

//    determineScanArea



//struct TWOSPOTAXESSTATEMACHINE {
//    TwoSpotAxesStates m_state;
//    TwoSpotAxesStates external_new_state;
//    bool external_state_change;

//    double m_firstXpos;
//    double m_firstYpos;
//    double m_secondXpos;
//    double m_secondYpos;

//    TwoSpotAxesStates getState() {
//        return this->m_state;
//    }
//    void setState(TwoSpotAxesStates state) {
//        this->m_state = state;
//    }

//    public:
//    double getFirstX() {
//        return m_firstXpos;
//    }
//    double getFirstY() {
//        return m_firstYpos;
//    }
//    double getSecondX() {
//        return m_secondXpos;
//    }
//    double getSecondY() {
//        return m_secondYpos;
//    }
//    void setFirstX(double x) {
//        m_firstXpos = x;
//    }
//    void setFirstY(double y) {
//        m_firstYpos = y;
//    }
//    void setSecondX(double x) {
//        m_secondXpos = x;
//    }
//    void setSecondY(double y) {
//        m_secondYpos = y;
//    }
//    void checkXDimensions() {
//        if (m_firstXpos > m_secondXpos) {
//            ScanSM.setX2(m_firstXpos);
//            ScanSM.setX1(m_secondXpos);
//        }
//        else {
//            ScanSM.setX1(m_firstXpos);
//            ScanSM.setX2(m_secondXpos);
//        }
//    }
//    void checkYDimensions() {
//        if (m_firstYpos > m_secondYpos) {
//            ScanSM.setY2(m_firstYpos);
//            ScanSM.setY1(m_secondYpos);
//        }
//        else {
//            ScanSM.setY1(m_firstYpos);
//            ScanSM.setY2(m_secondYpos);
//        }
//    }
//} TwoSpotSM;






//void MainWindow::closeSerialPort() {
//    MainStateMachine.setState(SHUTDOWN);
//    if (serial->isOpen()) {
//        resetCTL();
//        resetAxes();
//        serial->close();
//    }

//    ui->actionConnect->setEnabled(true);
//    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
//    logInfo("Port Disconnected");
//    showStatusMessage(tr("Disconnected"));
//}

//CTL Startup






//UI
//void MainWindow::RunCheckInput() {
//    if (RunRecipeOn && recipe.getAutoScanBool()) {
//        ScanSM.setState(SASM_STARTUP);
//    }
//    if (!TUNER.getAutoTune() || !RunRecipeOn) {
//        if (TUNER.getActualPosition() > 98) {
//            ui->MB_Right_Button->setEnabled(false);
//        }
//        else if (TUNER.getActualPosition() < 2) {
//            ui->MB_Left_Button->setEnabled(false);
//        }
//        else {
//            ui->MB_Right_Button->setEnabled(true);
//            ui->MB_Left_Button->setEnabled(true);
//        }
//    }
//    if (TUNER.getAutoTune() && UIHidden == false) {
//        ui->MB_Right_Button->hide();
//        ui->MB_Left_Button->hide();
//        ui->stepSizeBox->hide();
//        ui->stepLabel->hide();
//        UIHidden = true;
//    }
//    if (!TUNER.getAutoTune() && UIHidden == true) {
//        ui->MB_Right_Button->show();
//        ui->MB_Left_Button->show();
//        ui->stepSizeBox->show();
//        ui->stepLabel->show();
//        UIHidden = false;
//    }

//    if (TUNER.getAutoTuneCMDFlag()) {
//        if (TUNER.getAutoTune()) {
//            writeRequest("$8601%", 6); //SET_AUTO_MAN 0x86 //$860p% p=1 AutoMode, p=0 ManualMode
//            readData();
//            TUNER.setAutoTuneCMDFlag(false);
//            logInfo("Auto Tune : enabled");
//            ui->autotune_recipe->setText("ON");
//            ui->autotune_recipe->setStyleSheet("QPushButton { "
//                                               "font: 16pt 'Ubuntu Bold';"
//                                               "color: rgb(115, 210, 22);"
//                                               "}");

//        }
//        else {
//            writeRequest("$8600%", 6);
//            readData();
//            TUNER.setAutoTuneCMDFlag(false);
//            logInfo("Auto Tune : disabled");
//            ui->autotune_recipe->setText("OFF");
//            ui->autotune_recipe->setStyleSheet("QPushButton { "
//                                                "font: 16pt 'Ubuntu Bold';"
//                                                "color: rgb(220, 0, 20);"
//                                                "}");

//        }
//    }
//    if (RF.getReadyToLoad()) {
//        QString command;
//        command = "$42" +RF.getLoadedSetPointQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_RF_WATTS  $42xxxx% xxxx = Watts; resp[!42xxxx#]
//        readData();
//        RF.setReadyToLoad(false);
//    }
//    if (TUNER.getReadyToLoad()) {
//        QString command;
//        command = "$43" + TUNER.getLoadedSPQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_MS_POS  $43xxxx$ xxxx = Base10 MB Motor Pos; resp[!43xxxx#]
//        readData();
//        TUNER.setReadyToLoad(false);
//    }
//    if (MFC[1].getReadyToLoad()) {
//        QString command;
//        command = "$4101" + MFC[1].getLoadedFlowQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
//        readData();
//        MFC[1].setReadyToLoad(false);
//    }
//    if (MFC[2].getReadyToLoad()) {
//        QString command;
//        command = "$4102" + MFC[2].getLoadedFlowQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
//        readData();
//        MFC[2].setReadyToLoad(false);
//    }
//    if (MFC[3].getReadyToLoad()) {
//        QString command;
//        command = "$4103" + MFC[3].getLoadedFlowQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
//        readData();
//        MFC[3].setReadyToLoad(false);
//    }
//    if (MFC[4].getReadyToLoad()) {
//        QString command;
//        command = "$4104" + MFC[4].getLoadedFlowQStr() + "%";
//        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
//        readData();
//        MFC[4].setReadyToLoad(false);
//    }

//}
//void MainWindow::updateAxisPosition() {
//    ui->X_relative_PH->setText(X.getQStrPosition());
//    ui->Y_relative_PH->setText(Y.getQStrPosition());
//    ui->Z_relative_PH->setText(Z.getQStrPosition());
//}
//void MainWindow::updateStageStatus(QString statusNow, QString statusNext) {
//    ui->axisstatus->setText(statusNow);
//    ui->axisstatus_2->setText(statusNext);
//}
//void MainWindow::setHomeBtnText(QString text) {
//    ui->Home_button->setText(text);
//}
//void MainWindow::setTwoSpotBtnText(QString text) {
//    ui->twospot_button->setText(text);
//}
//void MainWindow::setScanBtnText(QString text) {
//    ui->scan_button->setText(text);
//}
//void MainWindow::updateTwoSpotXYText() {
//    ui->x1_recipe->setText(ScanSM.getminXString());
//    ui->x2_recipe->setText(ScanSM.getmaxXString());
//    ui->y1_recipe->setText(ScanSM.getminYString());
//    ui->y2_recipe->setText(ScanSM.getmaxYString());
//}
//double MainWindow::getThickness() {
//    bool ok;
//    return ui->thickness_recipe->toPlainText().toDouble(&ok);
//}
//double MainWindow::getGap() {
//    bool ok;
//    return ui->gap_recipe->toPlainText().toDouble(&ok);
//}
//double MainWindow::getOverlap() {
//    bool ok;
//    return ui->overlap_recipe->toPlainText().toDouble(&ok);
//}
//double MainWindow::getCycles() {
//    bool ok;
//    return ui->cycles_recipe->toPlainText().toDouble(&ok);
//}
//QString MainWindow::getXminRecipeQStr() {
//    return ui->x1_recipe->toPlainText();
//}
//QString MainWindow::getXmaxRecipeQStr() {
//    return ui->x2_recipe->toPlainText();
//}
//QString MainWindow::getYminRecipeQStr() {
//    return ui->y1_recipe->toPlainText();
//}
//QString MainWindow::getYmaxRecipeQStr() {
//    return ui->y2_recipe->toPlainText();
//}
//QString MainWindow::getThicknessQStr() {
//    return ui->thickness_recipe->toPlainText();
//}
//QString MainWindow::getGapQStr() {
//    return ui->gap_recipe->toPlainText();
//}
//QString MainWindow::getOverlapQStr() {
//    return ui->overlap_recipe->toPlainText();
//}
//QString MainWindow::getCyclesQStr() {
//    return ui->cycles_recipe->toPlainText();
//}
//void MainWindow::initActionsConnections() {
//    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
//    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
//    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);
//    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
//    connect(ui->actionDebug_Mode, &QAction::triggered, this, &MainWindow::debugMode);
//}
//void MainWindow::about() {
//    QMessageBox::about(this, tr("About Ontos 3 Interface"),
//                   tr("The <b>Ontos3 Interface</b> is the latest"
//                      "modern GUI for Plasma applications."));
//}
//void MainWindow::showStatusMessage(const QString &message) {
//    status->setText(message);
//}
//void MainWindow::setRecipeMB() {
//    ui->mb_recipe->setText(TUNER.getLoadedSPQStr());
//}
//void MainWindow::setRecipeRF() {
//    ui->RF_recipe->setText(RF.getLoadedSetPointQStr());
//}
//void MainWindow::MFC4RecipeFlow() {
//    ui->gas4_recipe_SLPM->setText(MFC[4].getLoadedFlowQStr());
//    ui->mfc4_recipe->setText(MFC[4].getLoadedFlowQStr());
//}
//void MainWindow::MFC3RecipeFlow() {
//    ui->gas3_recipe_SLPM->setText(MFC[3].getLoadedFlowQStr());
//    ui->mfc3_recipe->setText(MFC[3].getLoadedFlowQStr());
//}
//void MainWindow::MFC2RecipeFlow() {
//    ui->gas2_recipe_SLPM->setText(MFC[2].getLoadedFlowQStr());
//    ui->mfc2_recipe->setText(MFC[2].getLoadedFlowQStr());
//}
//void MainWindow::MFC1RecipeFlow() {
//    ui->gas1_recipe_SLPM->setText(MFC[1].getLoadedFlowQStr());
//    ui->mfc1_recipe->setText(MFC[1].getLoadedFlowQStr());
//}
//void MainWindow::getHeadTemp() {
//    ui->temp_LCD->display(plasmahead.getHeadTemp());
//    ui->Temp_bar->setValue(plasmahead.getHeadTempInt());
//}
//void MainWindow::MFC4ActualFlow() {
//    ui->gas4_actual_SLPM->setText(MFC[4].getActualFlow());
//    ui->gas4_progressbar->setValue(MFC[4].getActualFlowInt());
//}
//void MainWindow::MFC3ActualFlow() {
//    ui->gas3_actual_SLPM->setText(MFC[3].getActualFlow());
//    ui->gas3_progessbar->setValue(MFC[3].getActualFlowInt());
//}

//void MainWindow::MFC2ActualFlow() {
//    ui->gas2_actual_SLPM->setText(MFC[2].getActualFlow());
//    ui->gas2_progressbar->setValue(MFC[2].getActualFlowInt());
//}

//void MainWindow::MFC1ActualFlow() {
//    ui->gas1_actual_SLPM->setText(MFC[1].getActualFlow());
//    ui->gas1_progressbar->setValue(MFC[1].getActualFlowInt());
//}

//void MainWindow::displayReflectedPower() {
//    if (RunRecipeOn) {
//        ui->RefRF_Actual_LCD->display(RF.getReflectedWatts());
//        ui->RefRF_bar->setValue(RF.getReflectedWatts());
//    }
//    else {
//        ui->RefRF_Actual_LCD->display(0);
//        ui->RefRF_bar->setValue(0);
//    }
//}

//void MainWindow::displayRFValue() {
//    if (RunRecipeOn) {
//        ui->RF_Actual_LCD->display(RF.getActualWatts());
//        ui->RF_bar->setValue(RF.getActualWatts());
//    }
//    else {
//        ui->RF_Actual_LCD->display(0);
//        ui->RF_bar->setValue(0);
//    }

//}
//void MainWindow::displayTunerPosition() {
//    if (RunRecipeOn) {
//        ui->MB_Actual_LCD->display(TUNER.getActualPosition());
//        ui->MB_Pos_Bar->setValue(TUNER.getActualPosition());
//    }
//    else {
//        ui->MB_Actual_LCD->display(0);
//        ui->MB_Pos_Bar->setValue(0);
//    }
//}




// LOGGING

//BUTTONS


void MainWindow::debugMode() {
    DEBUG_MODE = !DEBUG_MODE;
}




void MainWindow::toggleAutoScanOn() {
    recipe.setAutoScan(true);
    recipe.setAutoScanFlag(true);
    logInfo("Auto Start Scan : enabled");
    ui->autoscan_recipe->setText("ON");
    ui->autoscan_recipe->setStyleSheet("QPushButton { "
                                   "font: 16pt 'Ubuntu Bold';"
                                   "color: rgb(115, 210, 22);"
                                   "}");
}
void MainWindow::toggleAutoScanOff() {
    recipe.setAutoScan(false);
    logInfo("Auto Start Scan : disabled");
    ui->autoscan_recipe->setText("OFF");
    ui->autoscan_recipe->setStyleSheet("QPushButton { "
                                       "font: 16pt 'Ubuntu Bold';"
                                       "color: rgb(220, 0, 20);"
                                       "}");

}
void MainWindow::on_init_button_clicked() {
    InitSM.setState(IASM_STARTUP);
}
void MainWindow::on_twospot_button_clicked() {
    if (TwoSpotSM.getState() == TSSM_IDLE) {
        TwoSpotSM.setState(TSSM_STARTUP);
    }
    else {
        TwoSpotSM.setState(TSSM_SHUTDOWN);
    }
}
void MainWindow::on_scan_button_clicked() {
    if (ScanSM.getState() == SASM_IDLE) {
        ScanSM.setState(SASM_STARTUP);
    }
    else {
        ScanSM.setState(SASM_SHUTDOWN);
    }
}
void MainWindow::on_Home_button_clicked() {
    if (HomeSM.getState() == HASM_IDLE) {
        HomeSM.setState(HASM_STARTUP);
    }
    else {
        HomeSM.setState(HASM_SHUTDOWN);
    }
}
void MainWindow::on_Joystick_button_toggled(bool checked) {
     if (checked) {
        toggleJoystickOn();
    }
    else {
        toggleJoystickOff();

    }
}
void MainWindow::on_Stagepins_button_toggled(bool checked) {
    if (checked) {
        togglePinsOn();
    }
    else {
        togglePinsOff();
    }
}
void MainWindow::on_vac_button_toggled(bool checked) {
    if (checked) {
        toggleVacOn();
    }
    else {
        toggleVacOff();
    }
}
void MainWindow::on_n2_purge_button_toggled(bool checked) {
    if (checked) {
        toggleN2PurgeOn();
    }
    else {
        toggleN2PurgeOff();
    }
}
void MainWindow::on_x1_set_clicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "X1 setpoint","X: (min) -" + stageBaseCoordinates.getXQStr() + " (max) " + stageBaseCoordinates.getXQStr(), 0, -stageBaseCoordinates.getXp2Base(), stageBaseCoordinates.getXp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setX1(value);
        ui->x1_recipe->setText(QString::number(value));
    }
}
void MainWindow::on_x2_set_clicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "X2 setpoint","X: (min) -" + stageBaseCoordinates.getXQStr() + " (max) " + stageBaseCoordinates.getXQStr(), 0, -stageBaseCoordinates.getXp2Base(), stageBaseCoordinates.getXp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setX2(value);
        ui->x2_recipe->setText(QString::number(value));
    }
}
void MainWindow::on_Y1_set_clicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "Y1 setpoint","Y: (min) -" + stageBaseCoordinates.getYQStr() + " (max) " + stageBaseCoordinates.getYQStr(), 0, -stageBaseCoordinates.getY(), stageBaseCoordinates.getY(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setY1(value);
        ui->y1_recipe->setText(QString::number(value));
    }
}
void MainWindow::on_Y2_set_clicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "Y2 setpoint","Y: (min) -" + stageBaseCoordinates.getYQStr() + " (max) " + stageBaseCoordinates.getYQStr(), 0, -stageBaseCoordinates.getY(), stageBaseCoordinates.getY(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setY2(value);
        ui->y2_recipe->setText(QString::number(value));
    }
}
void MainWindow::on_load_RF_clicked() {
    bool ok;
    int intVal = QInputDialog::getInt(this, "RF Setpoint","watts: ", 0, 0, 200, 0, &ok);
    if (ok) {
        RF.setLoadedSetPoint(QString::number(intVal));
        ui->RF_recipe->setText(RF.getLoadedSetPointQStr());
        ui->RF_bar->setValue(RF.getLoadedSetPoint() * 100);       
    }
}
void MainWindow::on_load_MB_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Matchbox Setpoint","%: ", 0, 0, 100.0, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        TUNER.setLoadedValue(QString::number(doubVal));
        ui->mb_recipe->setText(TUNER.getLoadedSPQStr());
        ui->MB_Pos_Bar->setValue(TUNER.getLoadedSP() * 100);        
    }
}
void MainWindow::on_load_MFC4_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 4 Setpoint","SLPM: ", 0, 0, 0.300, 3, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        MFC[4].setLoadedFlow(QString::number(doubVal));
        ui->mfc4_recipe->setText(MFC[4].getLoadedFlowQStr());
        ui->gas4_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 3));
        ui->gas4_sliderbar->setValue(doubVal * 100);
    }
}
void MainWindow::on_load_MFC3_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 3 Setpoint","SLPM: ", 0, 0, 0.300, 3, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        MFC[3].setLoadedFlow(QString::number(doubVal));
        ui->mfc3_recipe->setText(MFC[3].getLoadedFlowQStr());
        ui->gas3_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 3));
        ui->gas3_sliderbar->setValue(doubVal * 100);        
    }
}
void MainWindow::on_load_MFC2_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 2 Setpoint","SLPM: ", 0, 0, 20.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        MFC[2].setLoadedFlow(QString::number(doubVal));
        ui->mfc2_recipe->setText(MFC[2].getLoadedFlowQStr());
        ui->gas2_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 2));
        ui->gas2_sliderbar->setValue(doubVal * 100);       
    }
}
void MainWindow::on_load_MFC1_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "MFC 1 Setpoint","SLPM: ", 0, 0, 20.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        MFC[1].setLoadedFlow(QString::number(doubVal));
        ui->mfc1_recipe->setText(MFC[1].getLoadedFlowQStr());
        ui->gas1_recipe_SLPM->setText(QString().setNum(doubVal, 'F', 2));
        ui->gas1_sliderbar->setValue(doubVal * 100);
    }
}
void MainWindow::on_batchID_checkBox_stateChanged(int arg1) {
    if (arg1) {
        toggleBatchIDOn();
    }
    else {
        toggleBatchIDOff();
    }
}
void MainWindow::on_diameter_button_clicked() {
    RunDiameter();
}
void MainWindow::on_Heater_Button_toggled(bool checked) {
    if (checked) {
        toggleHeaterOn();
    }
    else {
        toggleHeaterOff();
    }
}
void MainWindow::on_load_thick_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Thickness: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        recipe.setThickness(doubVal);
        ui->thickness_recipe->setText(recipe.getThickness());
    }
}
void MainWindow::on_load_gap_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Gap: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        recipe.setGap(doubVal);
        ui->gap_recipe->setText(recipe.getGap());
    }
}
void MainWindow::on_load_overlap_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Overlap: ","mm: ", 0, 0, 5.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        recipe.setOverlap(doubVal);
        ui->overlap_recipe->setText(recipe.getOverlap());
    }
}
void MainWindow::on_load_speed_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Speed: ","mm: " + Param.getXMaxSpeedQStr(), 0, 0, Param.getXMaxSpeed(), 0, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        recipe.setSpeed(doubVal);
        ui->speed_recipe->setText(recipe.getSpeed());
    }
}
void MainWindow::on_load_cycles_clicked() {
    bool ok;
    double doubVal = QInputDialog::getInt(this, "Number of Cycles","cycles: ", 0, 0, 200, 0, &ok);
    if (ok) {
        recipe.setCycles(doubVal);
        ui->cycles_recipe->setText(recipe.getCycles());
    }
}
void MainWindow::on_plsmaBtn_toggled(bool checked) {
    if (checked) {
        turnOnExecRecipe();
    }
    else {
        turnOffExecRecipe();
    }
}

void MainWindow::on_load_recipe_button_clicked() {
    loadFromFile();
}
void MainWindow::on_save_recipe_button_clicked() {
    saveToFile();
}
void MainWindow::on_save_cascade_recipe_button_clicked()
{
    saveCascadeFile();
}
void MainWindow::on_load_cascade_recipe_button_clicked()
{
    loadCascadeFile();
}
void MainWindow::on_add_cascade_recipe_button_clicked()
{
    addCascadeFile();
}
void MainWindow::on_load_autotune_clicked() {
    if (TUNER.getAutoTune()) {
        toggleAutoTuneOff();
    }
    else {
        toggleAutoTuneOn();
    }
}
void MainWindow::on_load_autoscan_clicked() {
    if (recipe.getAutoScanBool()) {
        toggleAutoScanOff();
    }
    else {
        toggleAutoScanOn();
    }
}
void MainWindow::addCascadeFile() {
    QStringList StrVar;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Add recipe to Cascade List"), "/home/oem/Ontos3/Project_Linux_Migration/cascade_recipes/",
        tr("Recipe List (*.rcp);;All Files (*)"));
        if (!fileName.contains(".rcp")) {fileName += ".rcp";}
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;

            }
            StrVar = fileName.split("/");
            ui->cascadeList->addItem(StrVar.takeLast());
        }
}


void MainWindow::loadGUI(QStringList recipeData) {
    bool ok = false;
    int valueIndex;
    QStringList list;
    QStringList Params;
    QStringList Values;

    for (int i = 0; i < recipeData.size(); i++) {
        if (recipeData[i].isEmpty()) {break;}
        list = recipeData[i].split(":");
        Params.append(list[0]);
        Values.append(list[1]);
    }
    if (Params.contains("MFC1")) {
        valueIndex = Params.indexOf("MFC1");
        MFC[1].setLoadedFlow(Values[valueIndex]);
        ui->gas1_recipe_SLPM->setText(MFC[1].getLoadedFlowQStr());
        ui->mfc1_recipe->setText(MFC[1].getLoadedFlowQStr());
        ui->gas1_sliderbar->setValue(MFC[1].getLoadedFlow() * 100);
    }
    if (Params.contains("MFC2")) {
        valueIndex = Params.indexOf("MFC2");
        MFC[2].setLoadedFlow(Values[valueIndex]);
        ui->gas2_recipe_SLPM->setText(MFC[2].getLoadedFlowQStr());
        ui->mfc2_recipe->setText(MFC[2].getLoadedFlowQStr());
        ui->gas2_sliderbar->setValue(MFC[2].getLoadedFlow() * 100);
    }
    if (Params.contains("MFC3")) {
        valueIndex = Params.indexOf("MFC3");
        MFC[3].setLoadedFlow(Values[valueIndex]);
        ui->gas3_recipe_SLPM->setText(MFC[3].getLoadedFlowQStr());
        ui->mfc3_recipe->setText(MFC[3].getLoadedFlowQStr());
        ui->gas3_sliderbar->setValue(MFC[3].getLoadedFlow() * 100);
    }
    if (Params.contains("MFC4")) {
        valueIndex = Params.indexOf("MFC4");
        MFC[4].setLoadedFlow(Values[valueIndex]);
        ui->gas4_recipe_SLPM->setText(MFC[4].getLoadedFlowQStr());
        ui->mfc4_recipe->setText(MFC[4].getLoadedFlowQStr());
        ui->gas4_sliderbar->setValue(MFC[4].getLoadedFlow() * 100);
    }
    if (Params.contains("PWR")) {
        valueIndex = Params.indexOf("PWR");
        RF.setLoadedSetPoint(Values[valueIndex]);
        ui->RF_recipe->setText(RF.getLoadedSetPointQStr());
    }
    if (Params.contains("TUNER")) {
        valueIndex = Params.indexOf("TUNER");
        TUNER.setLoadedValue(Values[valueIndex]);
        ui->mb_recipe->setText(TUNER.getLoadedSPQStr());
    }
    if (Params.contains("AUTOTUNE")) {
        valueIndex = Params.indexOf("AUTOTUNE");
        TUNER.setAutoMode(Values[valueIndex]);
        if (TUNER.getAutoTune()) {
            toggleAutoTuneOn();
        }
        else {
            toggleAutoTuneOff();
        }
    }
    if (Params.contains("THICKNESS")) {
        valueIndex = Params.indexOf("THICKNESS");
        recipe.setThickness(Values[valueIndex].toInt(&ok));
        ui->thickness_recipe->setText(recipe.getThickness());
    }
    if (Params.contains("GAP")) {
        valueIndex = Params.indexOf("GAP");
        recipe.setGap(Values[valueIndex].toInt(&ok));
        ui->gap_recipe->setText(recipe.getGap());
    }
    if (Params.contains("OVERLAP")) {
        valueIndex = Params.indexOf("OVERLAP");
        recipe.setOverlap(Values[valueIndex].toInt(&ok));
        ui->overlap_recipe->setText(recipe.getOverlap());
    }
    if (Params.contains("SPEED")) {
        valueIndex = Params.indexOf("SPEED");
        recipe.setSpeed(Values[valueIndex].toInt(&ok));
        ui->speed_recipe->setText(recipe.getSpeed());
    }
    if (Params.contains("CYCLES")) {
        valueIndex = Params.indexOf("CYCLES");
        recipe.setCycles(Values[valueIndex].toInt(&ok));
        ui->cycles_recipe->setText(recipe.getCycles());
    }
    if (Params.contains("X2")) {
        valueIndex = Params.indexOf("X2");
        ScanSM.setX2(Values[valueIndex].toDouble(&ok));
        ui->x2_recipe->setText(QString::number(ScanSM.getX2()));
    }
    if (Params.contains("X1")) {
        valueIndex = Params.indexOf("X1");
        ScanSM.setX1(Values[valueIndex].toDouble(&ok));
        ui->x1_recipe->setText(QString::number(ScanSM.getX1()));
    }
    if (Params.contains("Y1")) {
        valueIndex = Params.indexOf("Y1");
        ScanSM.setY1(Values[valueIndex].toDouble(&ok));
        ui->y1_recipe->setText(QString::number(ScanSM.getY1()));
    }
    if (Params.contains("Y2")) {
        valueIndex = Params.indexOf("Y2");
        ScanSM.setY2(Values[valueIndex].toDouble(&ok));
        ui->y2_recipe->setText(QString::number(ScanSM.getY2()));
    }
    if (Params.contains("PURGE")) {
        valueIndex = Params.indexOf("PURGE");
        recipe.setPurge(Values[valueIndex].toInt(&ok));
        if (recipe.getPurge()) {
            ui->n2_purge_button->toggle();
        }
    }
    if (Params.contains("AUTOSCAN")) {
        valueIndex = Params.indexOf("AUTOSCAN");
        recipe.setAutoScan(Values[valueIndex].toInt(&ok));
        if (recipe.getAutoScanBool()) {
            toggleAutoScanOn();
        }
        else {
            toggleAutoScanOff();
        }
    }
    if (Params.contains("NAME")) {
        QStringList list;
        valueIndex = Params.indexOf("NAME");
        list = Values[valueIndex].split("/");
        ui->name_recipe->setText(list.takeLast());
    }

}
void MainWindow::loadConfigGUI(QStringList value) {
    config.setMFC1(value[1]);
    ui->gas1_label->setText(config.getMFC1());
    config.setMFC2(value[3]);
    ui->gas2_label->setText(config.getMFC2());
    config.setMFC3(value[5]);
    ui->gas3_label->setText(config.getMFC3());
    config.setMFC4(value[7]);
    ui->gas4_label->setText(config.getMFC4());

}

//State machines

void MainWindow::RunInitAxesSM() {
    switch (InitSM.getState()) {
        case IASM_STARTUP:
            //SM set to idle
            TwoSpotSM.setState(TSSM_IDLE);
            HomeSM.setState(HASM_IDLE);
            ScanSM.setState(SASM_IDLE);
            ScanSM.setSubState(SSSM_IDLE);
            //send commands
            stopMotors();
            setValve2("0");
            sendInitCMD();
            //GUI status
            ui->Stagepins_button->setChecked(true);
            updateStageStatus("Initializing Axes", "");
            //log
            logInfo("Initializing Axes");
            InitSM.setState(IASM_WAIT_FOR_DONE);
            break;
        case IASM_WAIT_FOR_DONE:
            if (nextStateReady()) {
                InitSM.setState(IASM_INITIALIZED);
            }
            break;
        case IASM_IDLE:
            //Do nothing
            break;
        case IASM_INITIALIZED:
            updateStageStatus("Stage Initialized", "");
            InitSM.setState(IASM_IDLE);
            break;
    }
}

void MainWindow::RunHomeAxesSM() {

    switch (HomeSM.getState()) {
        case HASM_STARTUP:
            InitSM.setState(IASM_IDLE);
            TwoSpotSM.setState(TSSM_IDLE);
            ScanSM.setState(SASM_IDLE);
            ScanSM.setSubState(SSSM_IDLE);
            setHomeBtnText("STOP");
            updateStageStatus("Homing Startup - Parking Z", "Homing X & Y");
            move("2", Param.getZMaxSpeedQStr(), stage.getPinsBuriedPosQStr()); //add function
            logInfo("Homing SM Start Up");
            HomeSM.setState(HASM_WAIT_PARK_Z);
            break;
        case HASM_WAIT_PARK_Z:
            if (nextStateReady()) {
                logInfo("Homing Z Parked");
                HomeSM.setState(HASM_HOME_XY);
            }
            break;
        case HASM_HOME_XY:
            updateStageStatus("Homing X & Y", "Homing Z");
            move("0", Param.getXMaxSpeedQStr(), Param.getXHomePosQStr());
            move("1", Param.getYMaxSpeedQStr(), Param.getYHomePosQStr());
            logInfo("Homing X & Y");
            HomeSM.setState(HASM_WAIT_HOME_XY);
            break;
        case HASM_WAIT_HOME_XY:
            if (nextStateReady()) {
                HomeSM.setState(HASM_HOME_Z);
                logInfo("Homing Z Parked");
            }
            break;
        case HASM_HOME_Z:
            updateStageStatus("Homing Z", "");
            logInfo("Homing Z Parked");
            move("2", Param.getZMaxSpeedQStr(), Param.getZHomePosQStr());
            HomeSM.setState(HASM_WAIT_HOME_Z);
            break;
        case HASM_WAIT_HOME_Z:
            if (nextStateReady()) {
                updateStageStatus("", "");
                setHomeBtnText("LOAD");
                logInfo("Z Homed");
                HomeSM.setState(HASM_WAIT_HOME_Z);
            }
            break;
        case HASM_SHUTDOWN:
            stopMotors();
            updateStageStatus("Operator Abort Homing", "");
            setHomeBtnText("LOAD");
            logInfo("Operator Abort Homing");
            move("2", Param.getZMaxSpeedQStr(), Param.getZHomePosQStr());
            updateStageStatus("", "");
            HomeSM.setState(HASM_IDLE);
            break;
        case HASM_IDLE:
            //DO NOTHING
            break;
    }
}
void MainWindow::RunTwoSpotAxesSM() {

    switch (TwoSpotSM.getState()) {
        case TSSM_STARTUP:
            enableJoy();
            setTwoSpotBtnText("STOP");
            updateStageStatus("Joy Stick is Enabled","Spot First Point");
            logInfo("TwoSpotSM Start Up");
            logInfo("TwoSpotSM Getting First");
            TwoSpotSM.setState(TSSM_GET_FIRST);
            break;
        case TSSM_GET_FIRST:
            if (joystickOn) {
                ui->axisstatus_2->setText("Release JoyStick Button");
                TwoSpotSM.setFirstX(X.getPositionRelative2Base());
                TwoSpotSM.setFirstY(Y.getPositionRelative2Base());
                logInfo("TwoSpotSM Got First");
                TwoSpotSM.setState(TSSM_WAIT_JOY_BTN_OFF);
            }
            break;
        case TSSM_WAIT_JOY_BTN_OFF:
            if (!joystickOn) {
                logInfo("TwoSpotSM Getting Second");
                updateStageStatus("Spot Second Point", "");
                TwoSpotSM.setState(TSSM_GET_SECOND);
            }
            break;
        case TSSM_GET_SECOND:
            if (joystickOn) {
                TwoSpotSM.setSecondX(X.getPositionRelative2Base());
                TwoSpotSM.setSecondY(Y.getPositionRelative2Base());
                //determine box orientation and corners
                TwoSpotSM.checkXDimensions();
                TwoSpotSM.checkYDimensions();
                updateTwoSpotXYText();
                logInfo("TwoSpotSM Got Second - done");
                TwoSpotSM.setState(TSSM_SHUTDOWN);
            }
            break;
        case TSSM_SHUTDOWN:
            disableJoy();
            TwoSpotSM.setState(TSSM_IDLE);
            updateStageStatus("", "");
            setTwoSpotBtnText("TWO SPOT");


        case TSSM_IDLE:
            //DO NOTHING
            break;
    }
}
void MainWindow::logStageScanInfo() {
    if (ScanSM.getCurrentCycle() == 1) {
            logInfo("-------------Scan Start-Up--------------"); }
    else
    {
            logInfo("-------------Scan Recycle Start-Up--------------");
            logInfo("This Cycle: " + ScanSM.getCurrentCycleString()); }

    logInfo("Display MIN:(" + ScanSM.getminXString() + " , " + ScanSM.getminYString() + ") MAX:(" + ScanSM.getmaxXString() + " , " + ScanSM.getmaxYString()  + ")");
    logInfo("Num Rows: " + ScanSM.getNumRowsString() + " Row Sweep: " + ScanSM.getScanSweepString());
    logInfo("FirstX: " + ScanSM.getStartingXPositionString() + " StartY: " + ScanSM.getStartingYPositionString() + " EndY: " + ScanSM.getEndingYPositionString());
    logInfo("Scan Speed: " + ScanSM.getScanSpeedString() + " Cycles: " + ScanSM.getScanCyclesString());
}
void MainWindow::RunScanAxesSM() {
    QString message;

    switch (ScanSM.getState()) {
        case SASM_STARTUP:
            setScanBtnText("STOP");

            ScanSM.determineSubstrateArea();
            ScanSM.determineStageZMinMax();
            ScanSM.determineScanSweep();
            ScanSM.calculateTotalScanXY();
            ScanSM.calculateNumScanSweeps();
            ScanSM.setCurrentRow(1);
            ScanSM.setCurrentCycle(1);
            ScanSM.calculateStartingPositions();
            ScanSM.setRecipeScanSpeed();
            logStageScanInfo();

            ScanSM.setState(SASM_SCAN);
            ScanSM.setSubState(SSSM_PARK_Z);
            updateStageStatus("Scanning", "");
            break;
        case SASM_SCAN:
            if (nextStateReady()) {

                switch (ScanSM.getSubState()) {
                    case SSSM_PARK_Z:
                        if (ScanSM.finishedScanning()) {
                            ScanSM.setSubState(SSSM_IDLE); //for completeness
                            ScanSM.setState(SASM_RECYCLE); //All done!
                            message = "End Cycle #" + ScanSM.getCurrentCycleString() + " of " + ScanSM.getScanCyclesString();
                            logInfo(message);
                            updateStageStatus("Scanning", message);
                            setScanBtnText("STOP");
                        }
                        else {
                            ScanSM.setSubState(SSSM_GO_XY_START);
                            message = "Parking Z";
                            logInfo(message);
                            updateStageStatus("", message);
                            setScanBtnText("STOP");
                        }
                        move("2", Param.getZMaxSpeedQStr(), ScanSM.getminZString());
                        break;                        
                    case SSSM_GO_XY_START:
                        message = "Scan Cycle #" + ScanSM.getCurrentCycleString() + " of " + ScanSM.getScanCyclesString() + " Traversal #" + ScanSM.getCurrentRowString() + " of " + ScanSM.getNumRowsString();
                        logInfo(message);
                        updateStageStatus("", message);
                        ScanSM.setNextSweepStartingPosition();

                        move("0", Param.getXMaxSpeedQStr(), ScanSM.getStartingXPositionString());
                        move("1", Param.getYMaxSpeedQStr(), ScanSM.getStartingYPositionString());
                        ScanSM.setSubState(SSSM_GO_Z_SCAN_POS);
                        break;
                    case SSSM_GO_Z_SCAN_POS:
                        if (recipe.getPurge()) {
                            writeRequest("$C701%", 6); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
                            readData();
                        }
                        move("2", Param.getZMaxSpeedQStr(), ScanSM.getmaxZString());
                        ScanSM.setSubState(SSSM_SCAN_COL);
                        break;
                    case SSSM_SCAN_COL:
                        move("1", ScanSM.getScanSpeedString(), ScanSM.getEndingYPositionString());
                        ScanSM.incrementNumRow();
                        ScanSM.setSubState(SSSM_PARK_Z);
                        break;
                    case SSSM_IDLE:
                        //do nothin
                        break;
                }
            }
            break;
        case SASM_RECYCLE:
            if (ScanSM.shouldContinueScanning()) {
                setValve2(0);
                setScanBtnText("SCAN");
                updateStageStatus("Scanning Completed", "");
                ScanSM.setState(SASM_IDLE);
                HomeSM.setState(HASM_STARTUP);
            }
            else {
                ScanSM.incrementCurrentCycle();
                ScanSM.setCurrentRow(1);
                ScanSM.calculateStartingPositions();
                ScanSM.setState(SASM_SCAN);
                ScanSM.setSubState(SSSM_GO_XY_START);
                logStageScanInfo();
            }
            break;
        case SASM_SHUTDOWN:
            if (ScanSM.getSubState() == SSSM_IDLE) {
                ScanSM.setState(SASM_IDLE);
            }
            else {
                updateStageStatus("Scanning Stopped - Parking Z", "");
                stopMotors();
                setValve2(0);
                move("2", Param.getZMaxSpeedQStr(), ScanSM.getminZString());
                ScanSM.setState(SASM_IDLE);
                ScanSM.setSubState(SSSM_IDLE);
                updateStageStatus("Scanning Manually Stopped", "");
                setScanBtnText("SCAN");
            }
            break;
        case SASM_IDLE:
            //DO NOTHING
            break;
    }
}

void MainWindow::RunDiameter() { //calculate scan recipe based on wafer diameter selected
        double wafer_diameter;
        double radius;
        double val;
        bool ok;
        //find the radius
        wafer_diameter = ui->wafer_diameter->currentText().toDouble(&ok);
        radius = wafer_diameter / 2.0;
        //find the points defining the box
        ScanSM.setX1(stageBaseCoordinates.getXp2Base() - radius);
        ScanSM.setX2(stageBaseCoordinates.getXp2Base() + radius);
        ScanSM.setY1(stageBaseCoordinates.getY() - radius);
        ScanSM.setY2(stageBaseCoordinates.getY() + radius);

        //update the "actual" display after coord sys translation
        val = XPos_RefB_2_RefPH(ScanSM.getX1());
        ui->x1_recipe->setText(QString::number(val));
        val = XPos_RefB_2_RefPH(ScanSM.getX2());
        ui->x2_recipe->setText(QString::number(val));

        val = YPos_RefB_2_RefPH(ScanSM.getY1());
        ui->y1_recipe->setText(QString::number(val));
        val = YPos_RefB_2_RefPH(ScanSM.getY2());
        ui->y2_recipe->setText(QString::number(val));


}





QString configuration::getMFC2() const
{
    return MFC2;
}

void configuration::setMFC2(const QString &newMFC2)
{
    MFC2 = newMFC2;
}

QString configuration::getMFC1() const
{
    return MFC1;
}

void configuration::setMFC1(const QString &newMFC1)
{
    MFC1 = newMFC1;
}

QString configuration::getMFC3() const
{
    return MFC3;
}

void configuration::setMFC3(const QString &newMFC3)
{
    MFC3 = newMFC3;
}

QString configuration::getMFC4() const
{
    return MFC4;
}

void configuration::setMFC4(const QString &newMFC4)
{
    MFC4 = newMFC4;
}

QString configuration::getExeConfigPath() const
{
    return ExeConfigPath;
}

QString configuration::getExeConfigPathFileName() const
{
    return ExeConfigPathFileName;
}


double StageBaseCoordinate::getPlasmaHeadZ() const
{
    return PlasmaHeadZ;
}

void StageBaseCoordinate::setPlasmaHeadZ(double newPlasmaHeadZ)
{
    PlasmaHeadZ = newPlasmaHeadZ;
}

double StageBaseCoordinate::getStageBaseY() const
{
    return stageBaseY;
}

void StageBaseCoordinate::setStageBaseY(double newStageBaseY)
{
    stageBaseY = newStageBaseY;
}

double StageBaseCoordinate::getStageBaseZ() const
{
    return stageBaseZ;
}

void StageBaseCoordinate::setStageBaseZ(double newStageBaseZ)
{
    stageBaseZ = newStageBaseZ;
}

double StageBaseCoordinate::getStageBaseX() const
{
    return stageBaseX;
}

void StageBaseCoordinate::setStageBaseX(double newStageBaseX)
{
    stageBaseX = newStageBaseX;
}

double LaserCoordinates::getLaser() const
{
    return Laser;
}

double LaserCoordinates::getPosition() const
{
    return position;
}
double getPosition() const;

double LaserCoordinates::getPosition() const
{
    return position;
}

double Coordinate::getPosition() const
{
    return position;
}

void Coordinate::setPosition(double newPosition)
{
    if (qFuzzyCompare(position, newPosition))
            return;
    position = newPosition;
    emit positionChanged();
}
