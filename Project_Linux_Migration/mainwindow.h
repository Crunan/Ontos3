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

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void debugMode();
    void handleError(QSerialPort::SerialPortError error);
    void mainLoopTick();
    void saveToFile();
    void loadFromFile();

    void on_actionService_triggered();
    void on_actionMFCLoadedValues_triggered();
    void on_MB_Left_Button_clicked();
    void on_MB_Right_Button_clicked();
    void on_autoMode_Button_clicked();
    void on_Heater_Button_clicked();
    void on_Send_CMD_button_clicked();
    void on_Clear_Button_clicked();
    void on_gas1_setpoint_button_clicked();
    void on_gas2_setpoint_button_clicked();
    void on_gas3_setpoint_button_clicked();
    void on_gas4_setpoint_button_clicked();
    void on_RF_Setpoint_Button_clicked();
    void on_MB_Setpoint_Button_clicked();
    void on_mb_Dial_valueChanged(int value);
    void on_RF_Dial_valueChanged(int value);
    void on_plsmaBtn_clicked();

private:
    Ui::MainWindow *ui = nullptr;
    QTimer *timer = nullptr;
    QLabel *status = nullptr;
    SettingsDialog *settings = nullptr;
    QSerialPort *serial = nullptr;

    //UI
    bool DEBUG_MODE = false;
    bool BatchLoggingOnOFF = false;

    void getBatchLogging();
    void hideServiceFunctions() const;
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void setConnectBtnColor(QString);
    void shutDownProgram();
    void loadGUI(QStringList RecipeParam);
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
    static constexpr int IDLE  = 0;
    static constexpr int STARTUP = 1;
    static constexpr int POLLING = 2;
    static constexpr int SHUTDOWN = 3;
    int SM_state = IDLE;
    int SM_PollCounter = 0;
    int SM_POLL_PERIOD = 10;
    void stateMachine();
    void RunCheckInput();
    void RunStartup();
    void RunPolling();    
    std::chrono::milliseconds CTLResetTimeOut = 0ms;
    //PLASMA/RECIPE
    bool ToggleRunRecipe = false;
    bool RunRecipeOn = false;
    bool SetDefaultRecipe = false;
    void turnOffExecRecipe();
    void setExecuteRecipe();
    //TEMP
    void GetTemp();
    void setTempValue();
    void displayTemperature();
    bool ToggleHeater = false;
    bool heaterModeOn = false;
    //MB
    void getAutoMan();
    void getRecipeMBPosition();
    bool Step_MB_Left = false;
    bool Step_MB_Right = false;
    bool ToggleAutoMode = false;
    bool AutoModeOn = false;
    QString MBRightSpeed = "$11010032%"; //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    QString MBLeftSpeed = "$11000032%"; //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
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
    void writeRequest(QByteArray const &data, qint64);
    void writeFromCMDbox();
    void resetCTL();
    //RCV
    QString RCV;
    QString CTLPCBStatus;
    QStringList CTL_ParametersAndValues;
    bool HandshakeStatusChanged = false;
    int HandshakeStatusBits = 0;
    int HandshakeStatusBitsWas = 0;

    char * responseChar{};
    bool CTLStatusChanged = false;
    void StatusBitsStrToHex();
    void setTunerPosition();
    void displayTunerPosition();
    int StatusBits = 0;
    int StatusBitsWas = 0;
    void getCTLStatus();
    bool didCTLStatusChange();
    void splitTheRCVInformation();
    void UpdateStatus(int myStatusBits);
    void UpdateHandshakeStatus(int myHandshakeStatusBits);
    void HandShakeStatusBitsStrToHex();
    void setRFPower();
    void displayRFValue();
    void readData();
    void setRCVbox(QString);        
    //LOG
    void logDebug(QString) const;
    void logInfo(QString) const;
    void logWarning(QString) const;
    void logCritical(QString) const;
    void logFatal() const;

};
#endif // MAINWINDOW_H
