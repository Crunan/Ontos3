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

struct configuration {
    QString ExeConfigPath = "./config/";
    QString ExeConfigPathFileName = "default";
    QString MFC1;
    QString MFC2;
    QString MFC3;
    QString MFC4;

    public:
    QString getMFC1() const;
    QString getMFC2() const;
    QString getMFC3() const;
    QString getMFC4() const;

    void setMFC1(const QString &newMFC1);
    void setMFC2(const QString &newMFC2);
    void setMFC3(const QString &newMFC3);
    void setMFC4(const QString &newMFC4);
    QString getExeConfigPath() const;
    QString getExeConfigPathFileName() const;

} config;

struct MFController {
public:
    double m_actualFlow;
    double m_loadedFlow;
    double m_range;
    bool m_readyToLoad;

    public:

    void setReadyToLoad(bool toggle) {
        m_readyToLoad = toggle;
    }
    bool getReadyToLoad() {
        return m_readyToLoad;
    }
    void setLoadedFlow(QString flow) {
        bool ok;
        m_loadedFlow = flow.toDouble(&ok);
        this->setReadyToLoad(true);
    }   
    void setRange(QString range) {
        bool ok;
        m_range = range.toDouble(&ok);
    }
    void setActualFlow(QString flow) {
        bool ok;
        m_actualFlow = flow.toDouble(&ok);
    }
    QString getLoadedFlowQStr() {
        return QString::number(m_loadedFlow, 'f', 2);
    }
    QString getRange() {
        return QString::number(m_range, 'f', 2);
    }
    QString getActualFlow() {
        return QString::number(m_actualFlow, 'f', 2);
    }
    int getActualFlowInt() {
        return int(m_actualFlow);
    }
    double getLoadedFlow() {
        return m_loadedFlow;
    }

} MFC[5];

struct PWR {
    public:
    int m_loadedSetPoint;
    int m_actualPForward;
    int m_actualPReflected;
    int m_maxWatts;
    bool m_readyToLoad;


    public:
    void setReadyToLoad(bool toggle) {
        m_readyToLoad = toggle;
    }
    bool getReadyToLoad() {
        return m_readyToLoad;
    }
    void setLoadedSetPoint(QString value) {
        bool ok;
        m_loadedSetPoint = value.toInt(&ok);
        this->setReadyToLoad(true);
    }
    void setMaxWatts(QString max) {
        bool ok;
        m_maxWatts = max.toInt(&ok);
    }
    void setActualWatts(QString watts) {
        bool ok;
        m_actualPForward = watts.toInt(&ok, 10);
    }
    void setActualRefWatts(QString watts) {
        bool ok;
        m_actualPReflected = watts.toInt(&ok, 10);
    }
    int getLoadedSetPoint() {
        return m_loadedSetPoint;
    }
    QString getLoadedSetPointQStr() {
        return QString::number(m_loadedSetPoint);
    }
    int getActualWatts() {
        return m_actualPForward;
    }
    int getReflectedWatts() {
        return m_actualPReflected;
    }
} RF;

struct TUNER_POS {
    public:
    double m_loadedSP;
    double m_actualPos;
    double m_actualPosPct;
    bool m_autoTune;
    bool m_autoTuneCMDFlag;
    bool m_readyToLoad;

    public:
    void setReadyToLoad(bool toggle) {
        m_readyToLoad = toggle;
    }
    bool getReadyToLoad() {
        return m_readyToLoad;
    }
    void setAutoTuneCMDFlag(bool toggle) {
        m_autoTuneCMDFlag = toggle;
    }
    void setLoadedValue(QString value) {
        bool ok;
        m_loadedSP = value.toDouble(&ok);
        this->setReadyToLoad(true);
    }
    void setAutoMode(QString value) {
        bool ok;
        m_autoTune = value.toInt(&ok);
        this->setAutoTuneCMDFlag(true);
    }
    void setActualPosition(QString pos) {
        bool ok;
        m_actualPos = pos.toDouble(&ok);
    }

    double getLoadedSP() {
        return m_loadedSP;
    }
    int getLoadedSPInteger() {
        return int(m_loadedSP);
    }
    bool getAutoTune() {
        return m_autoTune;
    }
    double getActualPosition() {
        return m_actualPos;
    }
    QString getLoadedSPQStr() {
        return QString::number(m_loadedSP);
    }
    QString getAutoTuneQStr() {
        return QString::number(m_autoTune);
    }
    bool getAutoTuneCMDFlag() {
        return m_autoTuneCMDFlag;
    }
} TUNER;

struct PlasmaHead {

    double m_safetyGap;  //Distance between plasma head & chuck when z at max height
    double m_plasmaHeadSlitLength; //Plasma head slit length
    double m_plasmaHeadSlitWidth;  //Plasma head slit width
    double m_headTemp;

    public:
    void setHeadTemp(QString temp) {
        bool ok;
        m_headTemp = temp.toDouble(&ok);
    }
    void setSafetyGap(QString gap) {
        bool ok;
        m_safetyGap = gap.toDouble(&ok);
    }
    void setPlasmaHeadSlitLength(QString length) {
        bool ok;
        m_plasmaHeadSlitLength = length.toDouble(&ok);
    }
    void setPlasmaHeadSlitWidth(QString width) {
        bool ok;
        m_plasmaHeadSlitWidth = width.toDouble(&ok);
    }
    double getPlasmaHeadSlitLength() {
        return m_plasmaHeadSlitLength;
    }
    double getPlasmaHeadSlitWidth() {
        return m_plasmaHeadSlitWidth;
    }
    double getSafetyGap() {
        return m_safetyGap;
    }
    double getHeadTemp() {
        return m_headTemp;
    }
    double getHeadTempInt() {
        return int(m_headTemp);
    }


} plasmahead;
//Axes SM
enum AxisStates {
    ASM_IDLE = 0x10,
    ASM_ABS_START = 0x11,
    ASM_ABS_WAIT = 0x12,
    ASM_REL_START = 0x13,
    ASM_REL_WAIT = 0x14,
    ASM_JOY_ON = 0x15
};
//Init Axes SM
enum InitStates {
    IASM_IDLE,
    IASM_STARTUP,
    IASM_WAIT_FOR_DONE,
    IASM_INITIALIZED
};
//Home Axes SM
enum HomeAxesStates {
    HASM_STARTUP,
    HASM_WAIT_PARK_Z,
    HASM_HOME_XY,
    HASM_WAIT_HOME_XY,
    HASM_HOME_Z,
    HASM_WAIT_HOME_Z,
    HASM_SHUTDOWN,
    HASM_IDLE
};
//Two Spot SM
enum TwoSpotAxesStates {
    TSSM_SHUTDOWN,
    TSSM_IDLE,
    TSSM_STARTUP,
    TSSM_GET_FIRST,
    TSSM_WAIT_JOY_BTN_OFF,
    TSSM_GET_SECOND
};
//Scan Axes SM
enum ScanAxesStates {
    SASM_IDLE,
    SASM_STARTUP,
    SASM_SCAN,
    SASM_RECYCLE,
    SASM_SHUTDOWN
};
//Scan Sub Axes SM
enum ScanSubStates {
    SSSM_IDLE,
    SSSM_PARK_Z,
    SSSM_GO_XY_START,
    SSSM_GO_Z_SCAN_POS,
    SSSM_SCAN_COL
};
//Light SM
enum lightstate {
    INACTIVE,
    ERROR,
    READY,
    ACTIVE
};
//Main SM
enum MachineStateMachine {
    STARTUP,
    POLLING,
    IDLE,
    SHUTDOWN
};

struct LIGHTSM {
    lightstate state;

    public:
    void setState(lightstate state) {
        this->state = state;
    }
} LightTower;

struct STATEMACHINE {
    MachineStateMachine m_state = IDLE;

    void setState(MachineStateMachine state) {
        m_state = state;
    }
    MachineStateMachine getState() {
        return m_state;
    }
} MainStateMachine;

struct PARAMETERS {
    int m_numMFCs;
    bool m_batchLogging;
    double m_maxXPos;
    double m_maxXSpeed;
    double m_homeXPos;
    double m_maxYPos;
    double m_maxYSpeed;
    double m_homeYPos;
    double m_maxZPos;
    double m_maxZSpeed;
    double m_homeZPos;

    public:
    void setMFCs(QString num) {
        bool ok;
        m_numMFCs = num.toInt(&ok);
    }
    void setBatchLogging(QString toggle) {
        bool ok;
        m_batchLogging = toggle.toInt(&ok);
    }
    void setXMaxPos(QString x) {
        bool ok;
        m_maxXPos = x.toDouble((&ok));
    }
    void setYMaxPos(QString y) {
        bool ok;
        m_maxYPos = y.toDouble((&ok));
    }
    void setZMaxPos(QString z) {
        bool ok;
        m_maxZPos = z.toDouble((&ok));
    }

    void setXHomePos(QString x) {
        bool ok;
        m_homeXPos = x.toDouble((&ok));
    }
    void setYHomePos(QString y) {
        bool ok;
        m_homeYPos = y.toDouble((&ok));
    }
    void setZHomePos(QString z) {
        bool ok;
        m_homeZPos = z.toDouble((&ok));
    }

    void setXMaxSpeed(QString speed) {
        bool ok;
        m_maxXSpeed = speed.toDouble((&ok));
    }
    void setYMaxSpeed(QString speed) {
        bool ok;
        m_maxYSpeed = speed.toDouble((&ok));
    }
    void setZMaxSpeed(QString speed) {
        bool ok;
        m_maxZSpeed = speed.toDouble((&ok));
    }
    double getXMaxPos() {
        return m_maxXPos;
    }
    double getYMaxPos() {
        return m_maxYPos;
    }
    double getZMaxPos() {
        return m_maxZPos;
    }

    double getXHomePos() {
        return m_homeXPos;
    }
    double getYHomePos() {
        return m_homeYPos;
    }
    double getZHomePos() {
        return m_homeZPos;
    }
    double getXMaxSpeed() {
        return m_maxXSpeed;
    }
    double getYMaxSpeed() {
        return m_maxYSpeed;
    }
    double getZMaxSpeed() {
        return m_maxZSpeed;
    }
    QString getXMaxSpeedQStr() {
        return QString::number(m_maxXSpeed, 'f', 2);
    }
    QString getYMaxSpeedQStr() {
        return QString::number(m_maxYSpeed, 'f', 2);
    }
    QString getZMaxSpeedQStr() {
        return QString::number(m_maxZSpeed, 'f', 2);
    }
    QString getXHomePosQStr() {
        return QString::number(m_homeXPos);
    }
    QString getYHomePosQStr() {
        return QString::number(m_homeYPos);
    }
    QString getZHomePosQStr() {
        return QString::number(m_homeZPos);
    }
} Param;

struct RECIPE {
    private:
        double m_thickness;
        double m_gap;
        double m_overlap;
        double m_speed;
        int m_cycles = 1;
        bool m_autoScan;
        bool m_N2PurgeRecipe;
        bool m_autoScanFlag;

    public:
    void setAutoScanFlag(bool toggle) {
        m_autoScanFlag = toggle;
    }
    bool getAutoScanFlag() {
        return m_autoScanFlag;
    }
    void setAutoScan(bool toggle) {
        m_autoScan = toggle;
    }
    void setPurge(bool toggle) {
        m_N2PurgeRecipe = toggle;
    }
    void setCycles(int cycles) {
        m_cycles = cycles;
    }
    void setSpeed(double speed) {
        m_speed = speed;
    }
    void setOverlap(double overlap) {
        m_overlap = overlap;
    }
    void setGap(double gap) {
        m_gap = gap;
    }
    void setThickness(double thickness) {
        m_thickness = thickness;
    }

    QString getCycles() {
        return QString::number(m_cycles);
    }
    int getCyclesInt() {
        return m_cycles;
    }
    QString getSpeed() {
        return QString::number(m_speed);
    }
    QString getOverlap() {
        return QString::number(m_overlap);
    }
    QString getGap() {
        return QString::number(m_gap);
    }
    QString getThickness() {
        return QString::number(m_thickness);
    }
    QString getAutoScan() {
        return QString::number(m_autoScan);
    }

    QString getPurgeQStr() {
        return QString::number(m_N2PurgeRecipe);
    }
    bool getAutoScanBool() {
        return m_autoScan;
    }
    bool getPurge() {
        return m_N2PurgeRecipe;
    }

} recipe;
struct STAGE {
    private:
    double m_pinsBuriedPos;  //Distance in Z from reset-0 to bury the lift pins
    double m_pinsExposedPos; //Distance in Z from reset-0 to expose the lift pins
    public:
    void setPinsBuried(QString pos) {
        bool ok;
        this->m_pinsBuriedPos = pos.toDouble(&ok);
    }
    void setPinsExposed(QString pos) {
        bool ok;
        this->m_pinsExposedPos = pos.toDouble(&ok);
    }
    double getPinsBuriedPos() {
        return m_pinsBuriedPos;
    }
    double getPinsExposedPos() {
        return m_pinsExposedPos;
    }
    QString getPinsBuriedPosQStr() {
        return QString::number(m_pinsBuriedPos);
    }
    QString getPinsExposedPosQStr() {
        return QString::number(m_pinsExposedPos);
    }

} stage;

struct COORD_SYS {
    //coordinates relative to stage base
    double m_Xp_2_base;
    double m_Yp_2_base;
    double m_Zp_2_base;

    double m_laserYpos;
    double m_laserXpos;

    public:
    //translate displayed PH X,Y,Z to the Base PH X,Y,Z (for motor moves)
    double TranslateCoordXPH2Base(double x) {
        return (m_Xp_2_base - x);
    }
    double TranslateCoordYPH2Base(double y) {
        return (m_Yp_2_base - y);
    }
    double TranslateCoordZPH2Base(double z) {
        return (z - m_Zp_2_base);
    }
    void setXp2Base(QString x) {
        bool ok;
        m_Xp_2_base = x.toDouble((&ok));
    }
    void setYp2Base(QString y) {
        bool ok;
        m_Yp_2_base = y.toDouble((&ok));
    }
    void setZp2Base(QString z) {
        bool ok;
        m_Zp_2_base = z.toDouble((&ok));
    }
    void setlaserX2Base(QString x) {
        bool ok;
        m_laserXpos = x.toDouble((&ok));
    }
    void setlaserY2Base(QString y) {
        bool ok;
        m_laserYpos = y.toDouble((&ok));
    }
    double getXp2Base() {
        return m_Xp_2_base;
        }
    double getYp2Base() {
        return m_Yp_2_base;
    }
    double getZp2Base() {
        return m_Zp_2_base;
    }
    QString getXp2BaseQStr() {
        return QString::number(m_Xp_2_base);
        }
    QString getYp2BaseQStr() {
        return QString::number(m_Yp_2_base);
    }
    QString getZp2BaseQStr() {
        return QString::number(m_Zp_2_base);
    }
    double getYLaser2Base() {
        return m_laserYpos;
    }

} CoordParam;

struct AXIS {
    private:
    int m_state;
    int m_error;
    double m_currentPos;

    public:
    void setState(QString state) {
        bool ok;
        m_state = state.toInt(&ok, 16);
    }
    void setError(QString error) {
        bool ok;
        m_error = error.toInt(&ok, 16);
    }
    void setPosition(QString pos) {
        bool ok;
        m_currentPos = pos.toDouble(&ok);
    }
    int getState() {
        return m_state;
    }
    int getError() {
        return m_error;
    }
    double getPosition() {
        return m_currentPos;
    }
    QString getQStrPosition() {
        return QString::number(m_currentPos);
    }

} X, Y, Z;


class AxisController {
    private:
    QString m_unparsedStatus;
    QStringList m_axisStatus;
    int m_LEDstates;
    bool m_statusChanged = false;
    bool m_doorsOpen;
    bool m_joystickOn;
    bool sameStateXYZ;
    bool m_vacuumOn;
    bool m_nitrogenPurgeOn;

} AxisCTL;


//Init Axes SM
struct INITAXESSTATEMACHINE {
    private:
    InitStates m_state;
    public:
    InitStates getState() {
        return m_state;
    }
    void setState(InitStates state) {
        m_state = state;
    }
} InitSM;

struct HOMEAXESSTATEMACHINE {
    private:
    HomeAxesStates m_state;
    public:
    HomeAxesStates getState() {
        return m_state;
    }
    void setState(HomeAxesStates state) {
        m_state = state;
    }
} HomeSM;

struct SCANAXESSTATEMACHINE {
    private:
    ScanAxesStates m_state;
    ScanSubStates m_subState;
    double m_minPerPH;
    double m_maxPerPH;
    double m_minXpos;
    double m_maxXpos;
    double m_minYpos;
    double m_maxYpos;
    double m_minZpos;
    double m_maxZpos;
    double m_rowXwidth;
    double m_colYlength;
    double m_startXpos;
    double m_startYpos;
    double m_endYpos;
    double m_scanYspeed = 1;
    int m_numXrows;
    double m_Xremaining;
    int m_thisXrow;
    int m_thisCycleNum;
    public:
    void setState(ScanAxesStates state) {
        m_state = state;
    }
    void setSubState(ScanSubStates state) {
        m_subState = state;
    }
    void setMinPerPH(double value) {
        m_minPerPH = value;
    }
    void setMaxPerPH(double value) {
        m_maxPerPH = value;
    }
    void setXMinPos(double x) {
        m_minXpos = x;
    }
    void setXMaxPos(double x) {
        m_maxXpos = x;
    }
    void setYMinPos(double y) {
        m_minYpos = y;
    }
    void setYMaxPos(double y) {
        m_maxYpos = y;
    }
    void setZMinPos(double z) {
        m_minZpos = z;
    }
    void setZMaxPos() {
        bool ok;
        m_maxZpos = CoordParam.getZp2Base() - recipe.getThickness().toDouble(&ok) - recipe.getGap().toDouble(&ok);
    }
    void setXRowWidth(double val1, double val2) {
        m_rowXwidth = val1 - val2;
    }
    void setXTotal() {
        m_Xremaining = this->getXmax() - this->getXmin();
    }
    void setXRemaining() {
        m_Xremaining = m_Xremaining - this->getRowWidth();
    }
    void setThisCycle(int thisCycle) {
        m_thisCycleNum = thisCycle;
    }
    void setThisXRow(int row) {
        m_thisXrow = row;
    }
    void setNumRows(int val) {
        m_numXrows = val;
    }
    void setXNewStartPos(double pos1, double pos2) {
        m_startXpos = pos1 - pos2;
    }
    void setYStartPos() {
        m_startYpos = this->getYmax() + plasmahead.getPlasmaHeadSlitWidth();
    }
    void setYEndPos() {
        m_endYpos = this->getYmin() + plasmahead.getPlasmaHeadSlitWidth();
    }
    void setYSpeed() {
        bool ok;
        m_scanYspeed = recipe.getSpeed().toDouble(&ok);
    }
    void incrementNumRows() {
        this->m_numXrows = this->getNumXrows() + 1;
    }
    void incrementThisRow() {
        this->m_thisXrow = this->getThisXRow() + 1;
    }
    void incrementCycles() {
        m_thisCycleNum += 1;
    }

//    void checkXCoordinateSystem(double xmin, double xmax) {
//        this->setMinPerPH(CoordParam.TranslateCoordXPH2Base(xmin));
//        this->setMaxPerPH(CoordParam.TranslateCoordXPH2Base(xmax));
//        this->checkXmmPerPH();
//    }
//    void checkYCoordinateSystem(double ymin, double ymax) {
//        this->setMinPerPH(CoordParam.TranslateCoordYPH2Base(ymin));
//        this->setMaxPerPH(CoordParam.TranslateCoordYPH2Base(ymax));
//        this->checkYmmPerPH();
//    }
    void setXSinglePass() {
        m_startXpos = (this->getXmax() + this->getXmin()) / 2;
    }
    void setXMutltiplePass() {
        m_startXpos = this->getXmax() - (this->getRowWidth() / 2);
    }
    void setSingleOrMultipleScan() {
        if (this->getNumXrows() == 1) {
            this->setXSinglePass();
        }
        else {
            this->setXMutltiplePass();
        }
    }

    ScanAxesStates getState() {
        return m_state;
    }
    ScanSubStates getSubState() {
        return m_subState;
    }
    QString getXminQStr() {
        return QString::number(m_minXpos);
    }
    QString getXmaxQStr() {
        return QString::number(m_maxXpos);
    }
    QString getYminQStr() {
        return QString::number(m_minYpos);
    }
    QString getYmaxQStr() {
        return QString::number(m_maxYpos);
    }
    QString getXRemainingQStr() {
        return QString::number(m_Xremaining);
    }
    QString getXNumRowsQStr() {
        return QString::number(m_numXrows);
    }
    QString getXRowWidthQStr() {
        return QString::number(m_rowXwidth);
    }
    QString getXThisRowQStr() {
        return QString::number(m_thisXrow);
    }
    QString getXStartPosQStr() {
        return QString::number(m_startXpos);
    }
    QString getYStartPosQStr() {
        return QString::number(m_startYpos);
    }
    QString getYEndPosQStr() {
        return QString::number(m_endYpos);
    }
    QString getScanYSpeedQStr() {
        return QString::number(m_scanYspeed);
    }
    QString getThisCyclesQStr() {
        return QString::number(m_thisCycleNum);
    }
    QString getZminQStr() {
        return QString::number(m_minZpos);
    }
    QString getZmaxQStr() {
        return QString::number(m_maxZpos);
    }
    double getXStartPos() {
        return m_startXpos;
    }
    double getXRowWidth() {
        return m_rowXwidth;
    }
    int getThisCycle() {
        return m_thisCycleNum;
    }

    double getXmin() {
        return m_minXpos;
    }
    double getXmax() {
        return m_maxXpos;
    }
    double getYmin() {
        return m_minYpos;
    }
    double getYmax() {
        return m_maxYpos;
    }
    double getMinPerPH() {
        return m_minPerPH;
    }
    double getMaxPerPH() {
        return m_maxPerPH;
    }
    double getXremaining() {
        return m_Xremaining;
    }
    double getNumXrows() {
        return m_numXrows;
    }
    double getRowWidth() {
        return m_rowXwidth;
    }
    int getThisXRow() {
        return m_thisXrow;
    }
    bool finishedScanning() {
        if (this->getThisXRow() > this->getNumXrows()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool keepScanning() {
        if (this->getThisCycle() >= recipe.getCyclesInt()) {
            return true;
        }
        else {
            return false;
        }
    }

} ScanSM;

struct TWOSPOTAXESSTATEMACHINE {
    TwoSpotAxesStates m_state;
    TwoSpotAxesStates external_new_state;
    bool external_state_change;

    double m_firstXpos;
    double m_firstYpos;
    double m_secondXpos;
    double m_secondYpos;

    TwoSpotAxesStates getState() {
        return this->m_state;
    }
    void setState(TwoSpotAxesStates state) {
        this->m_state = state;
    }

    public:
    double getFirstX() {
        return m_firstXpos;
    }
    double getFirstY() {
        return m_firstYpos;
    }
    double getSecondX() {
        return m_secondXpos;
    }
    double getSecondY() {
        return m_secondYpos;
    }
    void setFirstX(double x) {
        m_firstXpos = x;
    }
    void setFirstY(double y) {
        m_firstYpos = y;
    }
    void setSecondX(double x) {
        m_secondXpos = x;
    }
    void setSecondY(double y) {
        m_secondYpos = y;
    }
    void checkXDimensions() {
        if (m_firstXpos > m_secondXpos) {
            ScanSM.setXMaxPos(this->getFirstX());
            ScanSM.setXMinPos(this->getSecondX());
        }
        else {
            ScanSM.setXMinPos(this->getFirstX());
            ScanSM.setXMaxPos(this->getSecondX());
        }
    }
    void checkYDimensions() {
        if (m_firstYpos > m_secondYpos) {
            ScanSM.setYMaxPos(this->getFirstY());
            ScanSM.setYMinPos(this->getSecondY());
        }
        else {
            ScanSM.setYMinPos(this->getFirstY());
            ScanSM.setYMaxPos(this->getSecondY());
        }
    }
} TwoSpotSM;



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


void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
void MainWindow::closeSerialPort() {
    MainStateMachine.setState(SHUTDOWN);
    if (serial->isOpen()) {
        resetCTL();
        resetAxes();
        serial->close();
    }

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    logInfo("Port Disconnected");
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
        resetAxes();
        resetCTL();
        CTLResetTimeOut = 2500ms / timer->interval();
        (DEBUG_MODE) ? MainStateMachine.setState(IDLE) : MainStateMachine.setState(STARTUP);
        Logger::init();
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        showStatusMessage(tr("Open Port error"));
    }
}

void MainWindow::mainLoopTick() {
    static bool mainLoopRunning;
    if (mainLoopRunning)
        return;

    mainLoopRunning = true;

    runstateMachine();

    mainLoopRunning = false;
}
void MainWindow::shutDownProgram() {
    closeSerialPort();
    Logger::clean();
    MainWindow::close();
}
void MainWindow::runstateMachine() {
    switch(MainStateMachine.getState()) {
      case STARTUP:
        if (CTLResetTimeOut > 0ms) {
            CTLResetTimeOut -= 1ms;
            break;
        }
        else {
            logInfo("Main State Machine Start Up");
            RunStartup();
            MainStateMachine.setState(POLLING);
            UpdateStatus();
            break;
        }
      case POLLING:
        RunCheckInput();

        SM_PollCounter += 1;
        if (SM_PollCounter >= SM_POLL_PERIOD) {
            SM_PollCounter = 0;
            RunPolling();
            UpdateStatus();
            setLightTower();
            RunInitAxesSM();
            RunTwoSpotAxesSM();
            RunHomeAxesSM();
            RunScanAxesSM();

        }
        break;
      case IDLE:
        break;
      case SHUTDOWN:
        break;
        UpdateStatus(0);
    }
}
void MainWindow::CTLStartup() {
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
    turnOffExecRecipe();
    getTemp();
    setCTLStateMachinesIdle();
}
void MainWindow::RunStartup() {
    GetExeCfg();
    CTLStartup();
    AxisStartup();
}

void MainWindow::AxisStartup() {
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
void MainWindow::RunPolling() {
    getCTLStatus();
    //didCTLStatusChange(); //is this for logging?
    splitRCV();
    //! [0]
    setStatusBitsFromPoll();
    UpdateStatus();
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
    //! setExecRecipe()
    //! [5]
    setMFC1();
    MFC1ActualFlow();
    //! [6]
    setMFC2();
    MFC2ActualFlow();
    //! [7]
    setMFC3();
    MFC3ActualFlow();
    //! [8]
    setMFC4();
    MFC4ActualFlow();
    //! [9]
    setTempValue();
    getHeadTemp();
    //! [10]
    //UpdateHandshakeStatus();
    //! [11]
    getAxisStatus();
}
//CTL Startup
void MainWindow::howManyMFCs() {
    writeRequest("$2A002%", 7);
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7,1); //GET Number of MFCs (1-4) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
        if (StringIsValidIntChars(StrVar)) {
            Param.setMFCs(StrVar);
            logInfo("Number of MFC's: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not set # of MFCs, last requestData: " + requestData );

}
void MainWindow::getBatchIDLogging() {
    writeRequest("$2A011%", 7); //GET BatchIDLogging $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 1);
        if (StringIsValidIntChars(StrVar)) {
            Param.setBatchLogging(StrVar);
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
        QString StrVar = RCV.mid(7, 7);
        if (StringIsValidDoubleChars(StrVar)) {
            TUNER.setLoadedValue(StrVar);
            setRecipeMB();
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
            RF.setLoadedSetPoint(StrVar);
            setRecipeRF();
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
            MFC[4].setLoadedFlow(StrVar);
            MFC4RecipeFlow();
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
            MFC[3].setLoadedFlow(StrVar);
            MFC3RecipeFlow();
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
            MFC[2].setLoadedFlow(StrVar);
            MFC2RecipeFlow();
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
            MFC[1].setLoadedFlow(StrVar);
            MFC1RecipeFlow();
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
            MFC[4].setRange(StrVar);
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
            MFC[3].setRange(StrVar);
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
            MFC[2].setRange(StrVar);
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
            MFC[1].setRange(StrVar);
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
            RF.setMaxWatts(StrVar);
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
            TUNER.setAutoMode(StrVar);
            if (TUNER.getAutoTune()) {
                toggleAutoTuneOn();
            }
            else {
                toggleAutoTuneOff();
            }
            logInfo("Loaded Tuner Auto Setting: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve Auto/Manual setting, last requestData sent: " + requestData );
}
void MainWindow::getTemp() {
    writeRequest("$8C%", 4);
    readData();
    if (RCV.length() > 3) {
        QString StrVar = RCV.mid(3, 4);
        if (StringIsValidDoubleChars(StrVar)) {
            plasmahead.setHeadTemp(StrVar);
            getHeadTemp();
            logInfo("Loaded current temperature: " + StrVar);
        }
    else
        logCritical("Could Not retrieve temperature, last requestData sent: " + requestData );
    }
}
void MainWindow::setCTLStateMachinesIdle() {
    LightTower.setState(INACTIVE);
}
//CTL Commands
void MainWindow::resetCTL() {
    writeRequest("$90%", 4);
    readData();
}

void MainWindow::turnOnExecRecipe() {
    writeRequest("$8701%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
    readData();
    ui->plsmaBtn->setText("PLASMA OFF");
    logInfo("Execute Recipe : Enabled");
}
void MainWindow::turnOffExecRecipe() {
    writeRequest("$8700%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
    readData();
    ui->plsmaBtn->setText("START PLASMA");
    logInfo("Execute Recipe : Disabled");
}
void MainWindow::getCTLStatus() {
    writeRequest("$91%", 4); //GET_STATUS    $91% ; resp[!91LLRR#] LL = left LEDS, RR = right LEDS
    readData();
}
void MainWindow::splitRCV() {
    PCBStatus = RCV;
    CTLparametersAndValues = RCV.split(QLatin1Char(';'));
}
void MainWindow::setStatusBitsFromPoll() {
    bool ok;    
    QString StrVar = CTLparametersAndValues[0].mid(3);
    StatusBits = StrVar.toInt(&ok, 16);
}
void MainWindow::didStatusBitsChange() {
    if (StatusBits != StatusBitsWas)
        logInfo("Status Bits Change from " + QString::number(StatusBitsWas) + " to " + QString::number(StatusBits));
}
void MainWindow::UpdateStatus() {
        didStatusBitsChange();
        StatusBitsWas = StatusBits;

        (StatusBits & 0x0100) > 0 ? ui->actionGAS_1->setChecked(true) : ui->actionGAS_1->setChecked(false);
        (StatusBits & 0x0200) > 0 ? ui->actionGAS_2->setChecked(true) : ui->actionGAS_2->setChecked(false);
        (StatusBits & 0x0400) > 0 ? ui->actionGAS_3->setChecked(true) : ui->actionGAS_3->setChecked(false);
        (StatusBits & 0x0800) > 0 ? ui->actionGAS_4->setChecked(true) : ui->actionGAS_4->setChecked(false);

        (StatusBits & 0x1000) > 0 ? ui->actionV5->setChecked(true) : ui->actionV5->setChecked(false);
        (StatusBits & 0x2000) > 0 ? ui->actionV6->setChecked(true) : ui->actionV6->setChecked(false);
        (StatusBits & 0x4000) > 0 ? ui->actionV7->setChecked(true) : ui->actionV7->setChecked(false);
        (StatusBits & 0x8000) > 0 ? ui->actionRF_ENABLED->setChecked(true) : ui->actionRF_ENABLED->setChecked(false);

        (StatusBits & 0x0001) > 0 ? ui->actionPLASMA_ON->setChecked(true) : ui->actionPLASMA_ON->setChecked(false);
        (StatusBits & 0x0002) > 0 ? ui->actionTUNING->setChecked(true) : ui->actionTUNING->setChecked(false);
        (StatusBits & 0x0004) > 0 ? ui->actionAUTO_MODE->setChecked(true) : ui->actionAUTO_MODE->setChecked(false);
        (StatusBits & 0x0008) > 0 ? ui->actionEXECUTE_RECIPE->setChecked(true) : ui->actionEXECUTE_RECIPE->setChecked(false);

        (StatusBits & 0x0010) > 0 ? ui->actionESTOP_ON->setChecked(true) : ui->actionESTOP_ON->setChecked(false);
        (StatusBits & 0x0020) > 0 ? ui->actionDO_CMD->setChecked(true) : ui->actionDO_CMD->setChecked(false);
        (StatusBits & 0x0040) > 0 ? ui->actionHE_SIG->setChecked(true) : ui->actionHE_SIG->setChecked(false);
        (StatusBits & 0x0080) > 0 ? ui->actionPROCESS_ABORT->setChecked(true) : ui->actionPROCESS_ABORT->setChecked(false);

        if (ui->actionEXECUTE_RECIPE->isChecked()) {
            RunRecipeOn = true;
        }
        else {
            RunRecipeOn= false;
        }

}

void MainWindow::setMFC4() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[8])) {
        MFC[4].setActualFlow(CTLparametersAndValues[8]);
    };
}
void MainWindow::setMFC3() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[7])) {
        MFC[3].setActualFlow(CTLparametersAndValues[7]);
    };
}
void MainWindow::setMFC2() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[6])) {
        MFC[2].setActualFlow(CTLparametersAndValues[6]);
    };
}
void MainWindow::setMFC1() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[5])) {
        MFC[1].setActualFlow(CTLparametersAndValues[5]);
    };
}
void MainWindow::setReflectedPower() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[3])) {
        RF.setActualRefWatts(CTLparametersAndValues[3]);
    };
}
void MainWindow::setRFPower() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[2])) {
        RF.setActualWatts(CTLparametersAndValues[2]);
    };
}
void MainWindow::setTunerPosition() {
    if (StringIsValidDoubleChars(CTLparametersAndValues[1])) {
        TUNER.setActualPosition(CTLparametersAndValues[1]);
    };
}
void MainWindow::setTempValue() {
    plasmahead.setHeadTemp(CTLparametersAndValues[9]);
}
void MainWindow::setHandshakeStatus() {
    bool ok;
    if (StringIsValidHexChars(CTLparametersAndValues[10])) {
        HandshakeStatusBits = CTLparametersAndValues[10].toInt(&ok, 16);
    };
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
void MainWindow::setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble) {
    bool ok;
    LEDstates = firstHexStrNibble.toInt(&ok, 16); //First byte
    LEDstates = LEDstates<<8;
    LEDstates += secondHexStrNibble.toInt(&ok, 16); //First byte
}

//Axis Startup
void MainWindow::getXMaxSpeed() {
    writeRequest("$DA107%", 7); //GET_X_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setXMaxSpeed(StrVar);
            logInfo("X Max Speed: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get max speed for X, last requestData: " + requestData );
}

void MainWindow::getYMaxSpeed() {
    writeRequest("$DA207%", 7); //GET_Y_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setYMaxSpeed(StrVar);
            logInfo("Y Max Speed: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get max speed for Y, last requestData: " + requestData );
}
void MainWindow::getZMaxSpeed() {
    writeRequest("$DA307%", 7); //GET_Z_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setZMaxSpeed(StrVar);
            logInfo("Z Max Speed: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get max speed for Z, last requestData: " + requestData );
}
void MainWindow::getXp2Base() {
    writeRequest("$DA510%", 7); //GET Xp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            CoordParam.setXp2Base(StrVar);
            logInfo("Xp to Base: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get X relative to Base, last requestData: " + requestData );
}
void MainWindow::getYp2Base() {
    writeRequest("$DA520%", 7); //GET Yp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            CoordParam.setYp2Base(StrVar);
            logInfo("Yp to Base: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get Y relative to Base, last requestData: " + requestData );
}
void MainWindow::getZp2Base() {
    writeRequest("$DA530%", 7); //GET Zp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            CoordParam.setZp2Base(StrVar);
            logInfo("Zp to Base: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get Z relative to Base, last requestData: " + requestData );
}
void MainWindow::getXs2PH() {
    writeRequest("$DA511%", 7); //GET Xs_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            CoordParam.setlaserX2Base(StrVar);
            logInfo("Xs to Plasma Head: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get X relative to Plasma head, last requestData: " + requestData );
}
void MainWindow::getYs2PH() {
    writeRequest("$DA521%", 7); //GET Ys_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            CoordParam.setlaserY2Base(StrVar);
            logInfo("Ys to Plasma Head: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get Y relative to Plasma head, last requestData: " + requestData );
}
void MainWindow::getPHSlitLength() {
    writeRequest("$DA540%", 7); //GET Plasma Head Slit Length (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            plasmahead.setPlasmaHeadSlitLength(StrVar);
            logInfo("Plasma Head Slit Length: " + StrVar + " (mm)");
        }
    }
    else
        logCritical("Could Not get Plasma head slit length, last requestData: " + requestData );
}
void MainWindow::getPHSlitWidth() {
    writeRequest("$DA541%", 7); //GET Plasma Head Slit Width (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            plasmahead.setPlasmaHeadSlitWidth(StrVar);
            logInfo("Plasma Head Slit Width: " + StrVar + " (mm)");
        }
    }
    else
        logCritical("Could Not get Plasma head slit width, last requestData: " + requestData );
}
void MainWindow::getPHSafetyZGap() {
    writeRequest("$DA542%", 7); //GET Plasma Head Safety Gap $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            plasmahead.setSafetyGap(StrVar);
            logInfo("Plasma Head Z Safety Gap: " + StrVar + " (mm)");
        }
    }
    else
        logCritical("Could Not get Plasma Head Z Safety Gap, last requestData: " + requestData );
}
void MainWindow::getZPinsBuried() {
    writeRequest("$DA543%", 7); //GET Z Pins Buried Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            stage.setPinsBuried(StrVar);
            logInfo("Z Pins Buried Position: " + StrVar + " (mm)");
        }
    }
    else
        logCritical("Could Not get Z Pins Buried Position, last requestData: " + requestData );
}
void MainWindow::getZPinsExposed() {
    writeRequest("$DA544%", 7); //GET Z Pins Exposed Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            stage.setPinsExposed(StrVar);
            logInfo("Z Pins Exposed Position: " + StrVar + " (mm)");
        }
    }
    else
        logCritical("Could Not get Z Pins Exposed Position, last requestData: " + requestData );
}
void MainWindow::getLoadX2Base() {
    writeRequest("$DA512%", 7); //GET Load_X_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setXHomePos(StrVar);
            logInfo("X Load Position: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get X Load position, last requestData: " + requestData );
}
void MainWindow::getLoadY2Base() {
    writeRequest("$DA522%", 7); //GET Load_Y_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setYHomePos(StrVar);
            logInfo("Y Load Position: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get Y Load position, last requestData: " + requestData );
}
void MainWindow::getLoadZ2Base() {
    writeRequest("$DA532%", 7); //GET Load_Z_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            Param.setZHomePos(StrVar);
            logInfo("Z Load Position: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not get Z Load position, last requestData: " + requestData );
}
void MainWindow::setAxisStateMachinesIdle() {
    InitSM.setState(IASM_IDLE);
    HomeSM.setState(HASM_IDLE);
    TwoSpotSM.setState(TSSM_IDLE);
    ScanSM.setState(SASM_IDLE);
    ScanSM.setSubState(SSSM_IDLE);
}
//Axis Commands
void MainWindow::setDoors() {
    doorsOpen = isBitSet(LEDstates, 6);
}
void MainWindow::setJoyBtnOn() {
    joystickOn = isBitSet(LEDstates, 14);
}
void MainWindow::setVacOn() {
    vacuumOn = isBitSet(LEDstates, 12);
}
void MainWindow::setN2PurgeOn() {
    nitrogenPurgeOn = isBitSet(LEDstates, 11);
}
bool MainWindow::isJoyStickOn() {
    return joystickOn;
}
void MainWindow::updateAxisStatus() {
    //handle LED status
    setLEDstate(axisStatus[0], axisStatus[1]);
    //X
    X.setState(axisStatus[2]);
    X.setError(axisStatus[3]);
    X.setPosition(axisStatus[4]);
    //Y
    Y.setState(axisStatus[5]);
    Y.setError(axisStatus[6]);
    Y.setPosition(axisStatus[7]);
    //Z
    Z.setState(axisStatus[8]);
    Z.setError(axisStatus[9]);
    Z.setPosition(axisStatus[10]);
    //XYZ same? (probably stopped)
    setSameStateXYZsame();
    //update GUI positions
    updateAxisPosition();
    //set doors
    setDoors();
    setJoyBtnOn();
    setVacOn();
    setN2PurgeOn();
    //log any change
//    if (didStatusChange()) {
//        logAxesStatus();
//    }
}

void MainWindow::parseStatus() {
    QString statusToParse = RCV;
    statusToParse = statusToParse.mid(3); //remove the first 3 chars
    statusToParse = statusToParse.mid(0, statusToParse.length() -1); //remove # from response before split
    axisStatus = statusToParse.split(QLatin1Char(';')); //split string up
    updateAxisStatus();
}
void MainWindow::getAxisStatus() {
    writeRequest("$C0%", 4); //GET_STATUS $C0%; resp[!C0nn;nn;X state;X error;X pos; Y state; Y error, Y pos; Z state; Z error; Z pos#]
    readData();
    parseStatus();
}

void MainWindow::setSameStateXYZsame() {
    if ((X.getState() == Y.getState()) && (X.getState() == Z.getState())) {
        sameStateXYZ = true;
    }
    else {
        sameStateXYZ = false;
    }
}
bool MainWindow::nextStateReady() {
        if ((sameStateXYZ == true) && (X.getState() >= ASM_IDLE)) {
            return true;
        }
        else {
            return false;
        }
}
void MainWindow::setValve2(QString toggle) {
    writeRequest("$C70" + toggle + "%", 6); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
    readData();
}
void MainWindow::stopMotors() {
    writeRequest("$B3%", 4); //stop all motors
    readData();
}
void MainWindow::sendInitCMD() {
    writeRequest("$B500%", 6); //start initializing X axis
    readData();
    writeRequest("$B501%", 6); //start initializing Y axis
    readData();
    writeRequest("$B502%", 6); //start initializing Z axis
    readData();
}
void MainWindow::enableJoy() {
    writeRequest("$BE%", 4); //ENABLE_JOY $BE%; resp [!BE#];
    readData();
}
void MainWindow::disableJoy() {
    writeRequest("$BF%", 4); //DISABLE_JOY $BF%; resp [!BF#];
    readData();
}
void MainWindow::setSpeed(QString axis, QString speed) {
    QString command;
    command = "$B40" + axis + speed + "%";
    writeRequest(command, command.length());
    readData();
    logInfo("Move " + axis + " Speed: " + speed + " /sec ");
}
void MainWindow::setAbsMove(QString axis, QString position) {
    QString command;
    command = "$B60" + axis + position + "%";
    writeRequest(command, command.length());
    readData();
    logInfo("Move " + axis + " to: " + position);
}
void MainWindow::move(QString axis, QString speed, QString position) {
    setSpeed(axis, speed);
    setAbsMove(axis, position);
}
void MainWindow::resetAxes() {
    writeRequest("$A9%", 4);
    readData();
}
double MainWindow::getXCoord() {
    return XPos_RefB_2_RefPH(X.getPosition());
}
double MainWindow::getYCoord() {
    return YPos_RefB_2_RefPH(Y.getPosition());;
}

void MainWindow::logAxesStatus() {
    QString LogStr{};
    LogStr = "LED: " + axisStatus[0] + axisStatus[1];
    LogStr += " XSt: " + axisStatus[2];
    LogStr += " XPos: " + axisStatus[3];
    LogStr += " YSt: " + axisStatus[5];
    LogStr += " YPos: " + axisStatus[6];
    LogStr += " ZSt: " + axisStatus[8];
    LogStr += " ZPos: " + axisStatus[9];
    logInfo(LogStr);
}
//READ/WRITE
void MainWindow::writeRequest(QString CMD_Str, qint64 CMD_Len) {
    lastRequest = CMD_Str;
    RCV = "";
    ui->textCMDbox->appendPlainText(CMD_Str);
    serial->write(CMD_Str.toUtf8(), CMD_Len);
}

void MainWindow::readData() {
    if (serial->waitForReadyRead(1000)) {
        RCV = serial->readAll();
        while (serial->waitForReadyRead(100)) {
            if (!RCV.contains('#')) {
                RCV += serial->readAll();
            }
        }
        //logInfo(RCV);
        ui->textRCVbox->appendPlainText(RCV);
    }
}

//UI
void MainWindow::RunCheckInput() {
    if (RunRecipeOn && recipe.getAutoScanBool()) {
        ScanSM.setState(SASM_STARTUP);
    }
    if (!TUNER.getAutoTune() || !RunRecipeOn) {
        if (TUNER.getActualPosition() > 98) {
            ui->MB_Right_Button->setEnabled(false);
        }
        else if (TUNER.getActualPosition() < 2) {
            ui->MB_Left_Button->setEnabled(false);
        }
        else {
            ui->MB_Right_Button->setEnabled(true);
            ui->MB_Left_Button->setEnabled(true);
        }
    }
    if (TUNER.getAutoTune() && UIHidden == false) {
        ui->MB_Right_Button->hide();
        ui->MB_Left_Button->hide();
        ui->stepSizeBox->hide();
        ui->stepLabel->hide();
        UIHidden = true;
    }
    if (!TUNER.getAutoTune() && UIHidden == true) {
        ui->MB_Right_Button->show();
        ui->MB_Left_Button->show();
        ui->stepSizeBox->show();
        ui->stepLabel->show();
        UIHidden = false;
    }

    if (TUNER.getAutoTuneCMDFlag()) {
        if (TUNER.getAutoTune()) {
            writeRequest("$8601%", 6); //SET_AUTO_MAN 0x86 //$860p% p=1 AutoMode, p=0 ManualMode
            readData();
            TUNER.setAutoTuneCMDFlag(false);
            logInfo("Auto Tune : enabled");
            ui->autotune_recipe->setText("ON");
            ui->autotune_recipe->setStyleSheet("QPushButton { "
                                               "font: 16pt 'Ubuntu Bold';"
                                               "color: rgb(115, 210, 22);"
                                               "}");

        }
        else {
            writeRequest("$8600%", 6);
            readData();
            TUNER.setAutoTuneCMDFlag(false);
            logInfo("Auto Tune : disabled");
            ui->autotune_recipe->setText("OFF");
            ui->autotune_recipe->setStyleSheet("QPushButton { "
                                                "font: 16pt 'Ubuntu Bold';"
                                                "color: rgb(220, 0, 20);"
                                                "}");

        }
    }
    if (RF.getReadyToLoad()) {
        QString command;
        command = "$42" +RF.getLoadedSetPointQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_RF_WATTS  $42xxxx% xxxx = Watts; resp[!42xxxx#]
        readData();
        RF.setReadyToLoad(false);
    }
    if (TUNER.getReadyToLoad()) {
        QString command;
        command = "$43" + TUNER.getLoadedSPQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_MS_POS  $43xxxx$ xxxx = Base10 MB Motor Pos; resp[!43xxxx#]
        readData();
        TUNER.setReadyToLoad(false);
    }
    if (MFC[1].getReadyToLoad()) {
        QString command;
        command = "$4101" + MFC[1].getLoadedFlowQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        MFC[1].setReadyToLoad(false);
    }
    if (MFC[2].getReadyToLoad()) {
        QString command;
        command = "$4102" + MFC[2].getLoadedFlowQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        MFC[2].setReadyToLoad(false);
    }
    if (MFC[3].getReadyToLoad()) {
        QString command;
        command = "$4103" + MFC[3].getLoadedFlowQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        MFC[3].setReadyToLoad(false);
    }
    if (MFC[4].getReadyToLoad()) {
        QString command;
        command = "$4104" + MFC[4].getLoadedFlowQStr() + "%";
        writeRequest(command, command.length()); //SET_RCP_MFC_FLOW   $410mxxx.yy% 1<=m<=4, xxx.yy = flow rate; resp[!410mxxx.yy#]
        readData();
        MFC[4].setReadyToLoad(false);
    }

}
void MainWindow::updateAxisPosition() {
    ui->X_relative_PH->setText(X.getQStrPosition());
    ui->Y_relative_PH->setText(Y.getQStrPosition());
    ui->Z_relative_PH->setText(Z.getQStrPosition());
}
void MainWindow::updateStageStatus(QString statusNow, QString statusNext) {
    ui->axisstatus->setText(statusNow);
    ui->axisstatus_2->setText(statusNext);
}
void MainWindow::setHomeBtnText(QString text) {
    ui->Home_button->setText(text);
}
void MainWindow::setTwoSpotBtnText(QString text) {
    ui->twospot_button->setText(text);
}
void MainWindow::setScanBtnText(QString text) {
    ui->scan_button->setText(text);
}
void MainWindow::updateTwoSpotXYText() {
    ui->xmin_recipe->setText(ScanSM.getXminQStr());
    ui->xmax_recipe->setText(ScanSM.getXmaxQStr());
    ui->ymin_recipe->setText(ScanSM.getYminQStr());
    ui->ymax_recipe->setText(ScanSM.getYmaxQStr());
}
double MainWindow::getThickness() {
    bool ok;
    return ui->thickness_recipe->toPlainText().toDouble(&ok);
}
double MainWindow::getGap() {
    bool ok;
    return ui->gap_recipe->toPlainText().toDouble(&ok);
}
double MainWindow::getOverlap() {
    bool ok;
    return ui->overlap_recipe->toPlainText().toDouble(&ok);
}
double MainWindow::getCycles() {
    bool ok;
    return ui->cycles_recipe->toPlainText().toDouble(&ok);
}
QString MainWindow::getXminRecipeQStr() {
    return ui->xmin_recipe->toPlainText();
}
QString MainWindow::getXmaxRecipeQStr() {
    return ui->xmax_recipe->toPlainText();
}
QString MainWindow::getYminRecipeQStr() {
    return ui->ymin_recipe->toPlainText();
}
QString MainWindow::getYmaxRecipeQStr() {
    return ui->ymax_recipe->toPlainText();
}
QString MainWindow::getThicknessQStr() {
    return ui->thickness_recipe->toPlainText();
}
QString MainWindow::getGapQStr() {
    return ui->gap_recipe->toPlainText();
}
QString MainWindow::getOverlapQStr() {
    return ui->overlap_recipe->toPlainText();
}
QString MainWindow::getCyclesQStr() {
    return ui->cycles_recipe->toPlainText();
}
void MainWindow::initActionsConnections() {
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
    connect(ui->actionDebug_Mode, &QAction::triggered, this, &MainWindow::debugMode);
}
void MainWindow::about() {
    QMessageBox::about(this, tr("About Ontos 3 Interface"),
                   tr("The <b>Ontos3 Interface</b> is the latest"
                      "modern GUI for Plasma applications."));
}
void MainWindow::showStatusMessage(const QString &message) {
    status->setText(message);
}
void MainWindow::setRecipeMB() {
    ui->mb_recipe->setText(TUNER.getLoadedSPQStr());
}
void MainWindow::setRecipeRF() {
    ui->RF_recipe->setText(RF.getLoadedSetPointQStr());
}
void MainWindow::MFC4RecipeFlow() {
    ui->gas4_recipe_SLPM->setText(MFC[4].getLoadedFlowQStr());
    ui->mfc4_recipe->setText(MFC[4].getLoadedFlowQStr());
}
void MainWindow::MFC3RecipeFlow() {
    ui->gas3_recipe_SLPM->setText(MFC[3].getLoadedFlowQStr());
    ui->mfc3_recipe->setText(MFC[3].getLoadedFlowQStr());
}
void MainWindow::MFC2RecipeFlow() {
    ui->gas2_recipe_SLPM->setText(MFC[2].getLoadedFlowQStr());
    ui->mfc2_recipe->setText(MFC[2].getLoadedFlowQStr());
}
void MainWindow::MFC1RecipeFlow() {
    ui->gas1_recipe_SLPM->setText(MFC[1].getLoadedFlowQStr());
    ui->mfc1_recipe->setText(MFC[1].getLoadedFlowQStr());
}
void MainWindow::getHeadTemp() {
    ui->temp_LCD->display(plasmahead.getHeadTemp());
    ui->Temp_bar->setValue(plasmahead.getHeadTempInt());
}
void MainWindow::MFC4ActualFlow() {
    ui->gas4_actual_SLPM->setText(MFC[4].getActualFlow());
    ui->gas4_progressbar->setValue(MFC[4].getActualFlowInt());
}
void MainWindow::MFC3ActualFlow() {
    ui->gas3_actual_SLPM->setText(MFC[3].getActualFlow());
    ui->gas3_progessbar->setValue(MFC[3].getActualFlowInt());
}

void MainWindow::MFC2ActualFlow() {
    ui->gas2_actual_SLPM->setText(MFC[2].getActualFlow());
    ui->gas2_progressbar->setValue(MFC[2].getActualFlowInt());
}

void MainWindow::MFC1ActualFlow() {
    ui->gas1_actual_SLPM->setText(MFC[1].getActualFlow());
    ui->gas1_progressbar->setValue(MFC[1].getActualFlowInt());
}

void MainWindow::displayReflectedPower() {
    if (RunRecipeOn) {
        ui->RefRF_Actual_LCD->display(RF.getReflectedWatts());
        ui->RefRF_bar->setValue(RF.getReflectedWatts());
    }
    else {
        ui->RefRF_Actual_LCD->display(0);
        ui->RefRF_bar->setValue(0);
    }
}

void MainWindow::displayRFValue() {
    if (RunRecipeOn) {
        ui->RF_Actual_LCD->display(RF.getActualWatts());
        ui->RF_bar->setValue(RF.getActualWatts());
    }
    else {
        ui->RF_Actual_LCD->display(0);
        ui->RF_bar->setValue(0);
    }

}
void MainWindow::displayTunerPosition() {
    if (RunRecipeOn) {
        ui->MB_Actual_LCD->display(TUNER.getActualPosition());
        ui->MB_Pos_Bar->setValue(TUNER.getActualPosition());
    }
    else {
        ui->MB_Actual_LCD->display(0);
        ui->MB_Pos_Bar->setValue(0);
    }
}

//translate BASE X,Y,Z to the display-able PH X,Y,Z (for display)
double MainWindow::XPos_RefB_2_RefPH(double x) {
    return (CoordParam.getXp2Base() - x);
}
double MainWindow::YPos_RefB_2_RefPH(double y) {
    return (CoordParam.getYp2Base() - y);
}
double MainWindow::ZPos_RefB_2_RefPH(double z) {
    return (z - CoordParam.getZp2Base());
}

double MainWindow::Ys_PH(double y) {
    return (CoordParam.getYLaser2Base() - y);
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
bool MainWindow::isBitSet(int test_int, int bit_pos) {
    int bitmask{};

    bitmask = 1<<bit_pos;
    if (test_int & bitmask) {
        return true;
    }
    else {
        return false;
    }
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
//BUTTONS
void MainWindow::on_MB_Left_Button_clicked() {
    QString MBLeftSpeed = "$110000" + ui->stepSizeBox->currentText() + "%";
    writeRequest(MBLeftSpeed, MBLeftSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readData();
}

void MainWindow::on_MB_Right_Button_clicked() {
    QString MBRightSpeed = "$110100" + ui->stepSizeBox->currentText() + "%";
    writeRequest(MBRightSpeed, MBRightSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readData();
}
void MainWindow::on_Send_CMD_button_clicked() {
    QString StrVar = ui->text2sendCMDbox->toPlainText().toLocal8Bit();
    ui->text2sendCMDbox->clear();
    writeRequest(StrVar, StrVar.length());
    StrVar.clear();
}
void MainWindow::on_Clear_Button_clicked() {
    ui->textCMDbox->clear();
    ui->text2sendCMDbox->clear();
    ui->textRCVbox->clear();
}
void MainWindow::debugMode() {
    DEBUG_MODE = !DEBUG_MODE;
}
void MainWindow::toggleHeaterOn() {
    writeRequest("$CE35.0%", 8);
    readData();
    logInfo("Heater : enabled");
}

void MainWindow::toggleHeaterOff() {
    writeRequest("$CE00.0%", 8);
    readData();
    logInfo("Heater : disabled");
}
void MainWindow::toggleVacOn() {
    writeRequest("$C801%", 6); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
    readData();
    logInfo("Vac : enabled");
}

void MainWindow::toggleVacOff() {
    writeRequest("$C800%", 6); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
    readData();
    logInfo("Vac : disabled");
}
void MainWindow::toggleJoystickOn() {
    writeRequest("$BE%", 4);
    readData();
    logInfo("Joystick : enabled");
}

void MainWindow::toggleJoystickOff() {
    writeRequest("$BF%", 4);
    readData();
    logInfo("Joystick : disabled");
}

void MainWindow::toggleN2PurgeOn() {
    recipe.setPurge(true);
    logInfo("Recipe N2 purge : enabled");
}
void MainWindow::toggleN2PurgeOff() {
    recipe.setPurge(false);
    logInfo("Recipe N2 purge : disabled");
}
void MainWindow::togglePinsOn() {
    QString StrCmd{};
    QString pin_pos = QString::number(stage.getPinsExposedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    writeRequest(StrCmd, StrCmd.length());
    readData();
    logInfo("Stage Pins : enabled");
}
void MainWindow::togglePinsOff() {
    QString StrCmd{};
    QString pin_pos = QString::number(stage.getPinsBuriedPos());
    StrCmd = "$B602" +  pin_pos + "%";
    writeRequest(StrCmd, StrCmd.length());
    readData();
    logInfo("Stage Pins : disabled");
}
void MainWindow::toggleBatchIDOn() {
    writeRequest("$28011;1%", 9);
    readData();
    logInfo("Batch ID logging : enabled");
}
void MainWindow::toggleBatchIDOff() {
    writeRequest("$28011;0%", 9);
    readData();
    logInfo("Batch ID logging : disabled");
}
void MainWindow::toggleAutoTuneOn() {
    TUNER.setAutoMode("1");
}
void MainWindow::toggleAutoTuneOff() {
    TUNER.setAutoMode("0");

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
    double doubVal = QInputDialog::getDouble(this, "X minimum setpoint","X: (min) -" + CoordParam.getXp2BaseQStr() + " (max) " + CoordParam.getXp2BaseQStr(), 0, -CoordParam.getXp2Base(), CoordParam.getXp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setXMinPos(doubVal);
        ui->xmin_recipe->setText(QString::number(doubVal));
    }
}
void MainWindow::on_x2_set_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "X maximum setpoint","X: (min) -" + CoordParam.getXp2BaseQStr() + " (max) " + CoordParam.getXp2BaseQStr(), 0, -CoordParam.getXp2Base(), CoordParam.getXp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setXMaxPos(doubVal);
        ui->xmax_recipe->setText(QString::number(doubVal));
    }
}
void MainWindow::on_Y1_set_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Y minimum setpoint","Y: (min) -" + CoordParam.getYp2BaseQStr() + " (max) " + CoordParam.getYp2BaseQStr(), 0, -CoordParam.getYp2Base(), CoordParam.getYp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setYMinPos(doubVal);
        ui->ymin_recipe->setText(QString::number(doubVal));
    }
}
void MainWindow::on_Y2_set_clicked() {
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Y maximum setpoint","Y: (min) -" + CoordParam.getYp2BaseQStr() + " (max) " + CoordParam.getYp2BaseQStr(), 0, -CoordParam.getYp2Base(), CoordParam.getYp2Base(), 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        ScanSM.setYMaxPos(doubVal);
        ui->ymax_recipe->setText(QString::number(doubVal));
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
        tr("Add recipe to Cascade List"), "./cascade_recipes/",
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
void MainWindow::saveCascadeFile() {
    QStringList StrVar;
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Current Cascade Recipe"), "./cascade_recipes/",
        tr("Recipe List (*.crcp);;All Files (*)"));
        if (!fileName.contains(".crcp")) {fileName += ".crcp";}
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;

            }
            QTextStream out(&file);
            for(int i = 0; i > ui->cascadeList->count(); i++) {
                out<<ui->cascadeList->item(i);
            }
            file.close();
            StrVar = fileName.split("/");
            ui->cascade_recipe_name->setText(StrVar.takeLast());
        }
}
void MainWindow::loadCascadeFile() {
    QStringList cascade_recipe;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Recipe from File"), "./cascade_recipes/",
        tr("Recipe File (*.crcp);;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            QTextStream in(&file);
            while(!in.atEnd()) {
                QString line = in.readLine();
                cascade_recipe += line;
            }
            file.close();

            ui->cascadeList->addItems(cascade_recipe);
    }
}
void MainWindow::saveToFile() {
    QStringList StrVar;
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Current Recipe"), "./recipes/",
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
            QString RecipeString = "MFC1:" + MFC[1].getLoadedFlowQStr() + '\n' +
                                   "MFC2:" + MFC[2].getLoadedFlowQStr() + '\n' +
                                   "MFC3:" + MFC[3].getLoadedFlowQStr() + '\n' +
                                   "MFC4:" + MFC[4].getLoadedFlowQStr() + '\n' +
                                   "PWR:" + RF.getLoadedSetPointQStr() + '\n' +
                                   "TUNER:" + TUNER.getLoadedSPQStr() + '\n' +
                                   "AUTOTUNE:" + TUNER.getAutoTuneQStr() + '\n' +
                                   "THICKNESS:" + recipe.getThickness() + '\n' +
                                   "GAP:" + recipe.getGap() + '\n' +
                                   "OVERLAP:" + recipe.getOverlap() + '\n' +
                                   "SPEED:" + recipe.getSpeed() + '\n' +
                                   "CYCLES:" + recipe.getCycles() + '\n' +
                                   "XMIN:" + ScanSM.getXminQStr() + '\n' +
                                   "XMAX:" + ScanSM.getXmaxQStr() + '\n' +
                                   "YMIN:" + ScanSM.getYminQStr() + '\n' +
                                   "YMAX:" + ScanSM.getYmaxQStr() + '\n' +
                                   "PURGE:" + recipe.getPurgeQStr() + '\n' +
                                   "AUTOSCAN:" + recipe.getAutoScan();


            QTextStream out(&file);
            logInfo("Saved " + fileName + " : " + RecipeString);
            out << RecipeString.toUtf8();
            file.close();
            StrVar = fileName.split("/");
            ui->name_recipe->setText(StrVar.takeLast());
        }
}

void MainWindow::loadFromFile() {
    QStringList recipe;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Recipe from File"), "./recipes/",
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
            QTextStream in(&file);
            while(!in.atEnd()) {
                QString line = in.readLine();
                recipe += line;
            }
            recipe += "NAME:" + file.fileName();
            file.close();            
            loadGUI(recipe);
            showStatusMessage(tr("Loaded Recipe"));
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
    if (Params.contains("XMAX")) {
        valueIndex = Params.indexOf("XMAX");
        ScanSM.setXMaxPos(Values[valueIndex].toDouble(&ok));
        ui->xmax_recipe->setText(QString::number(XPos_RefB_2_RefPH(ScanSM.getXmax())));
    }
    if (Params.contains("XMIN")) {
        valueIndex = Params.indexOf("XMIN");
        ScanSM.setXMinPos(Values[valueIndex].toDouble(&ok));
        ui->xmin_recipe->setText(QString::number(XPos_RefB_2_RefPH(ScanSM.getXmin())));
    }
    if (Params.contains("YMIN")) {
        valueIndex = Params.indexOf("YMIN");
        ScanSM.setYMinPos(Values[valueIndex].toDouble(&ok));
        ui->ymin_recipe->setText(QString::number(YPos_RefB_2_RefPH(ScanSM.getYmin())));
    }
    if (Params.contains("YMAX")) {
        valueIndex = Params.indexOf("YMAX");
        ScanSM.setYMaxPos(Values[valueIndex].toDouble(&ok));
        ui->ymax_recipe->setText(QString::number(YPos_RefB_2_RefPH(ScanSM.getYmax())));
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
void MainWindow::GetExeCfg() {
    QStringList Values;
    QFile file("./config/default.cfg");

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        Values += line.split(">");
    }

    file.close();

    loadConfigGUI(Values);

}
//State machines
void MainWindow::setLightTower() {
    if ( ((GlobalmyStatusBits && 0x80) > 0) || (doorsOpen) ) {
        LightTower.state = ERROR;
    }
    else if ( (!doorsOpen) && (RunRecipeOn) ) {
        LightTower.state = ACTIVE;
    }
    else {
        if (LightTower.state != READY) {
            LightTower.state = READY;
        }
    }

    switch (LightTower.state) {
        case ERROR:
            writeRequest("$CB01%", 6); //$CB0n% resp[!CB0n#] n = 0,1,2,3 (none, red, amber, green)
            readData();
            LightTower.state = INACTIVE;
            break;
        case READY:
            writeRequest("$CB02%", 6);
            readData();
            LightTower.state = INACTIVE;
            break;
        case ACTIVE:
            writeRequest("$CB03%", 6);
            readData();
            LightTower.state = INACTIVE;
            break;
        case INACTIVE:
            //Do nothing
            break;

    }
}
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
                TwoSpotSM.setFirstX(CoordParam.TranslateCoordXPH2Base(getXCoord()));
                TwoSpotSM.setFirstY(CoordParam.TranslateCoordYPH2Base(getYCoord()));
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
                TwoSpotSM.setSecondX(CoordParam.TranslateCoordXPH2Base(getXCoord()));
                TwoSpotSM.setSecondY(CoordParam.TranslateCoordYPH2Base(getYCoord()));
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

void MainWindow::RunScanAxesSM() {
    QString message;

    bool ok;

    switch (ScanSM.getState()) {
        case SASM_STARTUP:
            setScanBtnText("STOP");
            ScanSM.setMinPerPH(ScanSM.getXmin());
            ScanSM.setMaxPerPH(ScanSM.getXmax());

            if (ScanSM.getMaxPerPH() > ScanSM.getMinPerPH()) {
                ScanSM.setXMaxPos(ScanSM.getMaxPerPH());
                ScanSM.setXMinPos(ScanSM.getMinPerPH());
            }
            else {
                ScanSM.setXMaxPos(ScanSM.getMinPerPH());
                ScanSM.setXMinPos(ScanSM.getMaxPerPH());
            }
            ScanSM.setMinPerPH(ScanSM.getYmin());
            ScanSM.setMaxPerPH(ScanSM.getYmax());

            if (ScanSM.getMaxPerPH() > ScanSM.getMinPerPH()) {
                ScanSM.setYMaxPos(ScanSM.getMaxPerPH());
                ScanSM.setYMinPos(ScanSM.getMinPerPH());
            }
            else {
                ScanSM.setYMaxPos(ScanSM.getMinPerPH());
                ScanSM.setYMinPos(ScanSM.getMaxPerPH());
            }

            ScanSM.setZMinPos(stage.getPinsBuriedPos());
            ScanSM.setZMaxPos();
            //Get the scan row info
            ScanSM.setXRowWidth(plasmahead.getPlasmaHeadSlitLength(), recipe.getOverlap().toDouble(&ok));
            ScanSM.setXTotal();
            ScanSM.setNumRows(0);
            while (ScanSM.getXremaining() >= 0) {
                ScanSM.incrementNumRows();
                ScanSM.setXRemaining();
            }
            ScanSM.setThisXRow(1);
            ScanSM.setThisCycle(1);
            ScanSM.setSingleOrMultipleScan();

            ScanSM.setYStartPos();
            ScanSM.setYEndPos();
            ScanSM.setYSpeed();
            logInfo("-------------Scan Start-Up--------------");
            logInfo("Display MIN:(" + ScanSM.getXminQStr() + " , " + ScanSM.getYminQStr() + ") MAX:(" + ScanSM.getXmaxQStr()+ " , " + ScanSM.getYmaxQStr()  + ")");
            logInfo("Num Rows: " + ScanSM.getXNumRowsQStr() + " Row Width: " + ScanSM.getXRowWidthQStr());
            logInfo("FirstX: " + ScanSM.getXStartPosQStr() + " StartY: " + ScanSM.getYStartPosQStr() + " EndY: " + ScanSM.getYEndPosQStr());
            logInfo("Scan Speed: " + recipe.getSpeed() + " Cycles: " + recipe.getCycles());
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
                            message = "End Cycle #" + ScanSM.getThisCyclesQStr() + " of " + recipe.getCycles();
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
                        move("2", Param.getZMaxSpeedQStr(), ScanSM.getZminQStr());
                        break;                        
                    case SSSM_GO_XY_START:
                        message = "Scan Cycle #" + ScanSM.getThisCyclesQStr() + " of " + recipe.getCycles() + " Traversal #" + ScanSM.getXThisRowQStr() + " of " + ScanSM.getXNumRowsQStr();
                        logInfo(message);
                        updateStageStatus("", message);
                        if (ScanSM.getThisXRow() > 1) {
                            ScanSM.setXNewStartPos(ScanSM.getXStartPos(), ScanSM.getXRowWidth());\
                        }
                        move("0", Param.getXMaxSpeedQStr(), ScanSM.getXStartPosQStr());
                        move("1", Param.getYMaxSpeedQStr(), ScanSM.getYStartPosQStr());
                        ScanSM.setSubState(SSSM_GO_Z_SCAN_POS);
                        break;
                    case SSSM_GO_Z_SCAN_POS:
                        if (recipe.getPurge()) {
                            writeRequest("$C701%", 6); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
                            readData();
                        }
                        move("2", Param.getZMaxSpeedQStr(), ScanSM.getZmaxQStr());
                        ScanSM.setSubState(SSSM_SCAN_COL);
                        break;
                    case SSSM_SCAN_COL:
                        move("1", ScanSM.getScanYSpeedQStr(), ScanSM.getYEndPosQStr());
                        ScanSM.incrementThisRow();
                        ScanSM.setSubState(SSSM_PARK_Z);
                        break;
                    case SSSM_IDLE:
                        //do nothin
                        break;
                }
            }
            break;
        case SASM_RECYCLE:
            if (ScanSM.keepScanning()) {
                setValve2(0);
                setScanBtnText("SCAN");
                updateStageStatus("Scanning Completed", "");
                ScanSM.setState(SASM_IDLE);
                HomeSM.setState(HASM_STARTUP);
            }
            else {
                ScanSM.incrementCycles();
                ScanSM.setThisXRow(1);
                ScanSM.setSingleOrMultipleScan();
                ScanSM.setState(SASM_SCAN);
                ScanSM.setSubState(SSSM_GO_XY_START);
                logInfo("-------------Scan Recycle Start-Up--------------");
                logInfo("This Cycle: " + ScanSM.getThisCyclesQStr());
                logInfo("Display MIN:(" + ScanSM.getXminQStr() + " , " + ScanSM.getYminQStr() + ") MAX:(" + ScanSM.getXmaxQStr()+ " , " + ScanSM.getYmaxQStr()   + ")");
                logInfo("Num Rows: " + ScanSM.getXNumRowsQStr() + " Row Width: " + ScanSM.getXRowWidthQStr());
                logInfo("FirstX: " + ScanSM.getXStartPosQStr() + " StartY: " + ScanSM.getYStartPosQStr() + " EndY: " + ScanSM.getYEndPosQStr());
                logInfo("Scan Speed: " + recipe.getSpeed() + " Cycles: " + recipe.getCycles());
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
                move("2", Param.getZMaxSpeedQStr(), ScanSM.getZminQStr());
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
        ScanSM.setXMinPos(CoordParam.getXp2Base() - radius);
        ScanSM.setXMaxPos(CoordParam.getXp2Base() + radius);
        ScanSM.setYMinPos(CoordParam.getYp2Base() - radius);
        ScanSM.setYMaxPos(CoordParam.getYp2Base() + radius);

        //update the "actual" display after coord sys translation
        val = XPos_RefB_2_RefPH(ScanSM.getXmin());
        ui->xmin_recipe->setText(QString::number(val));
        val = XPos_RefB_2_RefPH(ScanSM.getXmax());
        ui->xmax_recipe->setText(QString::number(val));

        val = YPos_RefB_2_RefPH(ScanSM.getYmin());
        ui->ymin_recipe->setText(QString::number(val));
        val = YPos_RefB_2_RefPH(ScanSM.getYmax());
        ui->ymax_recipe->setText(QString::number(val));


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

