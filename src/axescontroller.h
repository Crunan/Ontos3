#ifndef AXESCONTROLLER_H
#define AXESCONTROLLER_H

#include <QObject>
#include <QStateMachine>
#include "settingsdialog.h"
#include "serialinterface.h"
#include "axis.h"
#include "stage.h"
#include "configuration.h"

class QEventLoop;
class Recipe;

class AxesController : public QObject
{
    Q_OBJECT

public:
    explicit AxesController(QObject *parent = nullptr);
    ~AxesController();

    // serial port
    bool open(const SettingsDialog& settings);
    void close();
    bool sendCommand(const QString& command);
    QString readResponse();
    QString getPortErrorString();
    bool isOpen();
    void setSerialInterface(SerialInterface *interface);

    // state machines
    void StartInit() { emit ISM_TransitionStartup(); }
    void RunInitAxesSM();
    void StartHome() { emit HSM_TransitionStartup(); }
    void StopHome() { emit HSM_TransitionShutdown(); }
    void RunHomeAxesSM();
    void StartTwoSpot() { emit TSSM_TransitionStartup(); }
    void StopTwoSpot() { emit TSSM_TransitionShutdown(); }
    void RunTwoSpotSM();
    void setAxisStateMachinesIdle();
    bool nextStateReady();
    bool getAxesInitilizedStatus() { return m_axesInitialized; }
    void RunStageTestSM();
    bool axisStateMachineActive();

    // axis accessors
    void move(int axisCommandNum, float speed, float position);
    void stopAllMotors();
    void AxisStartup();
    void resetAxes();
    void getAxisStatus();
    void updateAxisStatus();
    // x axis
    int getXAxisState() { return m_Xaxis.getCurrentState(); }
    QString getXAxisMaxSpeedQStr() { return m_Xaxis.getMaxSpeedQStr(); }
    double XMaxSpeed() { return m_Xaxis.getMaxSpeed(); }
    QString getXHomePosQStr() { return m_Xaxis.getHomePosQStr(); }
    double getXPosition() const { return m_Xaxis.getPosition(); }
    int getXAxisError() const { return m_Xaxis.getError(); }

    // y axis
    int getYAxisState() { return m_Yaxis.getCurrentState(); }
    QString getYAxisMaxSpeedQStr() { return m_Yaxis.getMaxSpeedQStr(); }
    double getYPosition() const { return m_Yaxis.getPosition(); }
    int getYAxisError() const { return m_Yaxis.getError(); }
    double YMaxSpeed() const { return m_Yaxis.getMaxSpeed(); }

    // z axis
    int getZAxisState() { return m_Zaxis.getCurrentState(); }
    double getZPinsBuriedPos() { return m_stage.getPinsBuriedPos(); }
    double getZPosition() const { return m_Zaxis.getPosition(); }
    int getZAxisError() const { return m_Zaxis.getError(); }
    QString getZMaxSpeedQStr() const { return m_Zaxis.getMaxSpeedQStr(); }
    double ZMaxSpeed() const { return m_Zaxis.getMaxSpeed(); }

    // translate coordinates from base coordinates to PH coordinates
    double TranslateCoordXBase2PH(double Base_x){ return (m_Xp2Base - Base_x); }
    double TranslateCoordYBase2PH(double Base_y){ return (m_Yp2Base - Base_y); }
    double TranslateCoordZBase2PH(double Base_z){ return (Base_z - m_Zp2Base); }

    //translate displayed PH X,Y,Z to the Base PH X,Y,Z (for motor moves)
    double TranslateCoordXPH2Base(double PHx) { return (m_Xp2Base - PHx); }
    double TranslateCoordYPH2Base(double PHy) { return (m_Yp2Base - PHy); }
    double TranslateCoordZPH2Base(double PHz) { return (PHz + m_Zp2Base); }

    double TranslateCoordLaserY2PH(double Base_y) { return (m_Ys2PH - Base_y); }

    double getYs2PHval() { return m_Ys2PH; }
    double getXPH2Base() { return m_Xp2Base; }
    QString getXPH2BaseQStr() { return QString::number(m_Xp2Base, 'f', 2); }
    double getYPH2Base() { return m_Yp2Base; }
    QString getYPH2BaseQStr() { return QString::number(m_Yp2Base, 'f', 2); }
    double getZp2BaseDbl() { return m_Zp2Base; }

    // command wrappers
    void togglePinsOn();
    void togglePinsOff();
    void toggleJoystickOn();
    void toggleJoystickOff();
    void toggleN2PurgeOn();
    void toggleN2PurgeOff();
    void toggleVacOn();
    void toggleVacOff();

    // helper
    bool getDoorOpen() { return m_doorsOpen; }
    Configuration& getConfig() { return m_config; }

    // stage test
    void setDetailedLog(bool enabled) { m_detailedStageTestLogEnabled = enabled; }
    void setTestZ(bool enabled) { m_stageTestZEnabled = enabled; }

signals:

    void stageStatusUpdate(QString status1, QString status2);
    void updateUIAxisStatus();

    // init state machine UI updating signals
    void initSMStartup();
    void initSMDone();

    // home state machine UI updating signals
    void setUIHomeSMStartup();
    void setUIHomeSMDone();

    // two spot state machine UI updating signals
    void setUITwoSpotSMStartup();
    void setUITwoSpotSMDone();

    // init state machine transitions
    void ISM_TransitionWaitForDone();
    void ISM_TransitionInitialized();
    void ISM_TransitionIdle();
    void ISM_TransitionStartup();

    // home state machine transitions
    void HSM_TransitionStartupToWaitParkZ();
    void HSM_TransitionWaitParkZToHomeXY();
    void HSM_TransitionHomeXYToWaitHomeXY();
    void HSM_TransitionHomeXYToHomeZ();
    void HSM_TransitionHomeZToWaitHomeZ();
    void HSM_TransitionIdle();
    void HSM_TransitionStartup();
    void HSM_TransitionShutdown();

    // two spot state machine transitions
    void TSSM_TransitionGetFirst();
    void TSSM_TransitionJoyBtnOff();
    void TSSM_TransitionGetSecond();
    void TSSM_TransitionJoyBtnOff2();
    void TSSM_TransitionShutdown();
    void TSSM_TransitionIdle();
    void TSSM_TransitionStartup();

    // stage test state machine transitions
    void STSM_TransitionIdle();
    void STSM_TransitionShutdown();
    void STSM_TransitionStartup();
    void STSM_TransitionMaxX();
    void STSM_TransitionMinY();
    void STSM_TransitionMaxY();
    void STSM_TransitionMinX();

    // scan state machine to idle
    void ScanSM_TransitionIdle();

    // update ui
    void pinsStateChanged(bool state);
    void joystickStateChanged(bool state);
    void n2StateChanged(bool state);
    void vacStateChanged(bool state);
    void xLimitsChanged(double xmin, double xmax);
    void yLimitsChanged(double ymin, double ymax);

private:
    // status bit parsers
    void doorsStatus();
    void joyBtnStatus();
    void vacStatus();
    void N2PurgeStatus();

    // command wrappers.
    void getFirmwareVersion();
    void getXMaxSpeed();
    void getYMaxSpeed();
    void getZMaxSpeed();
    void getXMaxPosition();
    void getYMaxPosition();
    void getZMaxPosition();
    void getXp2Base();
    void getYp2Base();
    void getZp2Base();
    void getXs2PH();
    void getYs2PH();
    void getZPinsBuried();
    void getZPinsExposed();
    void getLoadX2Base();
    void getLoadY2Base();
    void getLoadZ2Base();

    // setup state machines
    void SetupInitAxesStateMachine();
    void SetupHomeAxesStateMachine();
    void SetupTwoSpotStateMachine();
    void SetupStageTestStateMachine();

    // utility functions
    void setValve2(QString toggle);
    void setValve2On();
    void setValve2Off();
    void stopMotors();
    void sendInitCMD();
    void setSameStateXYZsame();
    void parseStatus(QString serialResponse);
    QString getLastCommand() { return m_pSerialInterface->getLastCommand(); }
    void setLEDstate(QString firstHexStrNibble, QString secondHexStrNibble);
    void checkAndSetXDimensions();
    void checkAndSetYDimensions();
    void checkAndLogAxesStatusChange();
    void moveAxisAbsolute(int axisCommandNum, float targetPosition);
    void setAxisSpeed(int axisCommandNum, float targetSpeed);

    // stage test helper
    void stageTestZMax();
    void stageTestZMin();

    // init axes state machine
    QStateMachine m_initStateMachine;
    QState *m_pInitAxesIdleState;
    QState *m_pInitAxesStartupState;
    QState *m_pInitAxesWaitForDoneState;
    QState *m_pInitAxesInitializedState;

    // home axes state machine
    QStateMachine m_homeStateMachine;
    QState *m_pHomeAxisSuperState;
    QState *m_pHomeAxesStartupState;
    QState *m_pHomeAxesWaitParkZState;
    QState *m_pHomeAxesHomeXYState;
    QState *m_pHomeAxesWaitHomeXYState;
    QState *m_pHomeAxesHomeZState;
    QState *m_pHomeAxesWaitHomeZState;
    QState *m_pHomeAxesShutdownState;
    QState *m_pHomeAxesIdleState;

    // two spot state machine
    QStateMachine m_twoSpotStateMachine;
    QState *m_pTwoSpotSuperState;
    QState *m_pTwoSpotStartupState;
    QState *m_pTwoSpotGetFirstState;
    QState *m_pTwoSpotWaitJoyBtnOffState;
    QState *m_pTwoSpotGetSecondState;
    QState *m_pTwoSpotWaitJoyBtnOff2State;
    QState *m_pTwoSpotShutdownState;
    QState *m_pTwoSpotIdleState;

    // stage test state machine
    QStateMachine m_stageTestStateMachine;
    QState *m_pStageTestSuperState;
    QState *m_pStageTestIdleState;
    QState *m_pStageTestStartupState;
    QState *m_pStageTestMaxXState;
    QState *m_pStageTestMinXState;
    QState *m_pStageTestMaxYState;
    QState *m_pStageTestMinYState;
    QState *m_pStageTestShutdownState;

    // two spot variables
    double m_twoSpotXFirstPoint;
    double m_twoSpotXSecondPoint;
    double m_twoSpotYFirstPoint;
    double m_twoSpotYSecondPoint;

    // stage test variables
    bool m_stageTestZEnabled;
    bool m_detailedStageTestLogEnabled;
    int m_stageTestXCount;
    int m_stageTestYCount;
    int m_stageTestZCount;

    SerialInterface *m_pSerialInterface;

    Configuration m_config;

    Axis m_Xaxis, m_Yaxis, m_Zaxis;
    Stage m_stage;

    QStringList m_axisStatus;

    int m_LEDstates;
    bool m_sameStateXYZ;
    bool m_joystickOn;
    bool m_joyButtonOn;
    bool m_doorsOpen;
    bool m_N2PurgeOn;
    bool m_vacOn;

    bool m_axesInitialized;

    // coordinate transforms
    double m_Xp2Base;
    double m_Yp2Base;
    double m_Zp2Base;
    double m_Xs2PH;
    double m_Ys2PH;
};

#endif // AXESCONTROLLER_H
