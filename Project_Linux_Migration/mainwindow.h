#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QTimer>
#include <chrono>

using namespace std::literals;

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void logStageScanInfo();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void debugMode();
    void handleError(QSerialPort::SerialPortError error);
    void mainLoopTick();
    void saveToFile();
    void loadFromFile();



private:
    Ui::MainWindow *ui = nullptr;
    QTimer *timer = nullptr;
    QLabel *status = nullptr;
    SettingsDialog *settings = nullptr;
    QSerialPort *serial = nullptr;


    //UI
    bool DEBUG_MODE = false;
    bool BatchLoggingOn = false;
    bool UIHidden = false;
    void getBatchIDLogging();
    void toggleBatchIDOn();
    void toggleBatchIDOff();
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void setConnectBtnColor(QString);
    void shutDownProgram();

    bool StringIsValidIntChars(QString) const;
    bool StringIsValidDoubleChars(QString) const;
    bool StringIsValidHexChars(QString) const;
    QString DecIntToDecStr(int DecInt, int numChar);
    //GUI Inputs
    void checkStepMBLeftInput();
    void checkStepMBRightInput();
    void checkMFC4LoadFlowInput();
    void checkMFC3LoadFlowInput();
    void checkMFC2LoadFlowInput();
    void checkMFC1LoadFlowInput();
    void checkRFLoadPowerInput();
    void checkMBTunerPosInput();
    void checkAutoModeInput();
    void checkRunRecipeInput();
    void checkSetDefaultRecipeInput();
    //PORTS
    QList<QSerialPortInfo> SerialPorts;
    QSerialPort* serialPort;
    void getSerialPortsAndAddToPortBox();
    void connectOrDisconnectSerial();
    void CloseExistingSerial();
    void NewSerialConnection();
    void attemptConnection();
    //STATE MACHINE
    struct STATEMACHINE;
    struct LIGHTSM;
    int SM_PollCounter = 0;
    int SM_POLL_PERIOD = 10;
    void runstateMachine();
    void setLightTower();
    void RunCheckInput();
    void RunStartup();
    void RunPolling();    
    std::chrono::milliseconds CTLResetTimeOut = 0ms;
    //PLASMA/RECIPE   
    bool RunRecipeOn = false;
    void turnOffExecRecipe();
    void turnOnExecRecipe();
    //TEMP
    void setTempValue();
    void displayTemperature();

    //MB
    void getAutoMan();
    void getRecipeMBPosition();

    //RF
    void setReflectedPower();
    void displayReflectedPower();
    void getRecipeRFPosition();
    void getMaxRFPowerForward();
    //MFCs
    int NumMFC;
    void setMFC1();
    void displayMFC1();
    void setMFC2();
    void displayMFC2();
    void setMFC3();
    void displayMFC3();
    void setMFC4();
    void displayMFC4();
    void howManyMFCs();
    void getRecipeMFC4Flow();
    void getRecipeMFC3Flow();
    void getRecipeMFC2Flow();
    void getRecipeMFC1Flow();
    void getMFC4Range();
    void getMFC3Range();
    void getMFC2Range();
    void getMFC1Range();
    //CMD
    QString BinInt2String(int Bits16);
    QString requestData;
    QString lastRequest;
    void setCMDbox(QString);

    void writeFromCMDbox();
    void resetCTL();
    //RCV


    char * responseChar{};
    void StatusBitsStrToHex();
    void setTunerPosition();
    void displayTunerPosition();
    int GlobalmyStatusBits{};
    int StatusBits;
    int StatusBitsWas;
    void getCTLStatus();
    void splitTheRCVInformation();
    void UpdateStatus(int myStatusBits);
    void UpdateHandshakeStatus(int myHandshakeStatusBits);
    void HandShakeStatusBitsStrToHex();
    void setRFPower();
    void displayRFValue();
    void readData();
    void setRCVbox(QString);        
    //3 Axis Controls
    QStringList axisStatus;
    QStringList CTLparametersAndValues;

    void InitAxesSM();
    void RunInitAxesSM();
    bool isBitSet(int test_int, int bit_pos);
    void toggleVacOn();
    void toggleVacOff();
    void toggleHeaterOn();
    void toggleHeaterOff();
    void toggleJoystickOn();
    void toggleJoystickOff();
    void togglePinsOn();
    void togglePinsOff();
    void toggleN2PurgeOn();
    void toggleN2PurgeOff();
    void toggleAutoScanOn();
    void toggleAutoScanOff();

    //LOG
    void logDebug(QString) const;
    void logInfo(QString) const;
    void logWarning(QString) const;
    void logCritical(QString) const;
    void logFatal() const;

    double XPos_RefB_2_RefPH(double x);
    double YPos_RefB_2_RefPH(double y);
    double TranslateCoordZBase2PH(double z);
    double XPos_RefPH_2_RefB(double x);
    double YPos_RefPH_2_RefB(double y);
    double ZPos_RefPH_2_RefB(double z);
    double TranslateCoordYLaser2PH(double y);
    void RunDiameter();

    void AxisStartup();
    void getZMaxSpeed();
    void getYMaxSpeed();
    void getXMaxSpeed();
    void getPlasmaHeadZCoordinate();
    void getPlasmaHeadYCoordinate();
    void getPlasmaHeadXCoordinate();
    void getPlasmaLaserXCoordinate();
    void getPlasmaLaserYCoordinate();
    void getPHSlitLength();
    void getPHSlitWidth();
    void getPHSafetyZGap();
    void getZPinsBuried();
    void getZPinsExposed();
    void getLoadZ2Base();
    void getLoadY2Base();
    void getLoadX2Base();
    void RunHomeAxesSM();
    void RunTwoSpotAxesSM();
    void RunScanAxesSM();
    void toggleAutoTuneOn();
    void toggleAutoTuneOff();
    void showServiceFunctions() const;
    void resetAxes();
    QStringList SplitAxesResponse(QString StrVar);
    void axesXYZsame();
    void axesLEDs();
    void logAxesStatus(QStringList axes_status_list);
    void axesGUIposition();
    void getHeadTemp();
    void MFC4RecipeFlow();
    void MFC3RecipeFlow();    
    void MFC1RecipeFlow();
    void MFC2RecipeFlow();
    void MFC4ActualFlow();
    void MFC3ActualFlow();
    void MFC2ActualFlow();
    void MFC1ActualFlow();
    void logAxesStatus();
    void setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble);
    void setUnparsedStatus(QString rcv);
    void setDoors();
    void setJoyBtnOn();
    void setVacOn();
    void setN2PurgeOn();

    bool isJoyStickOn();
    double getThickness();
    double getOverlap();
    double getCycles();
    double getYmaxRecipe();
    double getYminRecipe();
    double getXmaxRecipe();
    double getXminRecipe();
    void updateTwoSpotXYText();
    void setScanBtnText(QString text);
    void setTwoSpotBtnText(QString text);
    void setHomeBtnText(QString text);
    void updateStageStatus(QString statusNow, QString statusNext);
    //void writeRequest(QByteArray CMD_Str, qint64 CMD_Len);
    void getTemp();
    void setCTLStateMachinesIdle();
    void setAxesStateMachinesIdle();
    void stopMotors();
    void sendInitCMD();
    void CTLStartup();
    void splitRCV();
    void setStatusBitsFromPoll();
    void setStatusChanged();
    void didStatusBitsChange();
    void UpdateStatus();
    bool didStatusChange();
    void setHandshakeStatus();
    void getAxisStatus();
    void updateAxisStatus();
    void setAxisStateMachinesIdle();
    void setSameStateXYZsame();

    bool joystickOn;
    bool doorsOpen;
    bool vacuumOn;
    bool nitrogenPurgeOn;
    int LEDstates;

    int firstXpos;
    int firstYpos;
    int secondXpos;
    int secondYpos;

    bool sameStateXYZ;
    bool nextStateReady();
    void enableJoy();
    void disableJoy();
    void setValve2(QString toggle);
    void setSpeed(QString axis, QString speed);
    void setAbsMove(QString axis, QString position);
    void move(QString axis, QString speed, QString position);
    void checkYCoordinateSystem();
    void checkXCoordinateSystem();
    QString getCyclesQStr();
    QString getOverlapQStr();
    QString getGapQStr();
    QString getThicknessQStr();
    QString getYmaxRecipeQStr();
    QString getYminRecipeQStr();
    QString getXmaxRecipeQStr();
    QString getXminRecipeQStr();

    void updateAxisPosition();
    void writeRequest(QString CMD_Str, qint64 CMD_Len);

    double getXCoord();
    double getYCoord();
    void GetExeCfg();
    void loadConfigGUI(QStringList value);
    void loadGUI(QStringList recipeData);
    void setRecipeMB();
    void setRecipeRF();
    void hideMBButtons();
    void saveCascadeFile();
    void addCascadeFile();
    void loadCascadeFile();
};
#endif // MAINWINDOW_H
