#ifndef AXESCONTROLLER_H
#define AXESCONTROLLER_H

#include <QObject>
#include <QStateMachine>
#include "settingsdialog.h"
#include "serialinterface.h"
#include "axis.h"
#include "stage.h"
#include "ledstatus.h"

class QEventLoop;
class Recipe;

class AxesController : public QObject
{
    Q_OBJECT

public:
    explicit AxesController(QObject *parent = nullptr);
    ~AxesController();

    bool open(const SettingsDialog& settings);
    void close();
    bool sendCommand(const QString& command);
    QString readResponse();

    QString getPortErrorString();
    bool isOpen();
    void setSerialInterface(SerialInterface *interface);

    void StartInit() { emit ISM_TransitionStartup(); }
    void RunInitAxesSM();
    void StartHome() { emit HSM_TransitionStartup(); }
    void StopHome() { emit HSM_TransitionShutdown(); }
    void RunHomeAxesSM();
    void StartTwoSpot() { emit TSSM_TransitionStartup(); }
    void StopTwoSpot() { emit TSSM_TransitionShutdown(); }
    void RunTwoSpotSM();

    void setXMaxPos(const double maxPos);
    void setXMaxSpeed(const double maxSpeed);
    void setXHomePos(const double homePos);

    void setYMaxPos(const double maxPos);
    void setYMaxSpeed(const double maxSpeed);
    void setYHomePos(const double homePos);

    void setZMaxPos(const double maxPos);
    void setZMaxSpeed(const double maxSpeed);
    void setZHomePos(const double homePos);

    int getXAxisState() { return m_Xaxis.getCurrentState(); }
    int getYAxisState() { return m_Yaxis.getCurrentState(); }
    int getZAxisState() { return m_Zaxis.getCurrentState(); }

    QString getXAxisMaxSpeedQStr() { return m_Xaxis.getMaxSpeedQStr(); }
    QString getYAxisMaxSpeedQStr() { return m_Yaxis.getMaxSpeedQStr(); }

    double getZPinsBuriedPos() { return m_stage.getPinsBuriedPos(); }

    void AxisStartup();

    QString getXMaxSpeedQStr() { return m_Xaxis.getMaxSpeedQStr(); }
    double XMaxSpeed() { return m_Xaxis.getMaxSpeed(); }
    QString getXHomePosQStr() { return m_Xaxis.getHomePosQStr(); }

    double getXTwoSpotFirstPoint() { return m_Xaxis.getTwoSpotFirstPoint(); }
    double getXTwoSpotSecondPoint() { return m_Xaxis.getTwoSpotSecondPoint(); }
    double getYTwoSpotFirstPoint() { return m_Yaxis.getTwoSpotFirstPoint(); }
    double getYTwoSpotSecondPoint() { return m_Yaxis.getTwoSpotSecondPoint(); }

    void setAxisStateMachinesIdle();

    void getAxisStatus();
    void updateAxisStatus();

    double getXPosition() const { return m_Xaxis.getPosition(); }
    double getYPosition() const { return m_Yaxis.getPosition(); }
    double getZPosition() const { return m_Zaxis.getPosition(); }
    int getXAxisError() const { return m_Xaxis.getError(); }
    int getYAxisError() const { return m_Yaxis.getError(); }
    int getZAxisError() const { return m_Zaxis.getError(); }
    QString getZMaxSpeedQStr() const { return m_Zaxis.getMaxSpeedQStr(); }

    //translate displayed PH X,Y,Z to the Base PH X,Y,Z (for motor moves)
    double TranslateCoordXPH2Base(double x) { return (m_Xp2Base - x); }
    double TranslateCoordYPH2Base(double y) { return (m_Yp2Base - y); }
    double TranslateCoordZPH2Base(double z) { return (z - m_Zp2Base); }
    double TranslateCoordY2PH(double y) { return  m_Ys2PH - y; }

    double getYs2PHval() { return m_Ys2PH; }
    double getXPH2Base() { return m_Xp2Base; }
    double getZp2BaseDbl() { return m_Zp2Base; }

    void resetAxes();
    bool nextStateReady();

    void togglePinsOn();
    void togglePinsOff();
    void toggleJoystickOn();
    void toggleJoystickOff();
    void toggleN2PurgeOn();
    void toggleN2PurgeOff();
    void toggleVacOn();
    void toggleVacOff();

    // helper
    bool isBitSet(int test_int, int bit_pos);
    bool getDoorStatus() { return m_doorsOpen; }

signals:

    void stageStatusUpdate(QString status1, QString status2);
    void stageResponseReceived(QString resonse);

    // init state machine UI updating signals
    void setUIInitSMStartup();
    void setUIInitSMDone();

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

    // scan state machine to idle
    void ScanSM_TransitionIdle();

    // update ui
    void pinsStateChanged(bool state);
    void joystickStateChanged(bool state);
    void n2StateChanged(bool state);
    void vacStateChanged(bool state);

private slots:
    // state machine on entry handlers
    void InitIdleOnEntry();
    void TwoSpotIdleOnEntry();
    void HomeIdleOnEntry();

private:
    // status bit parsers
    void doorsStatus();
    void joyBtnStatus();
    void vacStatus();
    void N2PurgeStatus();

    // command wrappers.
    void getXMaxSpeed();
    void getYMaxSpeed();
    void getZMaxSpeed();
    void getXp2Base();
    void getYp2Base();
    void getZp2Base();
    void getXs2PH();
    void getYs2PH();
    void getPHSafetyZGap();
    void getZPinsBuried();
    void getZPinsExposed();
    void getLoadX2Base();
    void getLoadY2Base();
    void getLoadZ2Base();

    // setup state machines
    void SetupInitAxesStateMachine();
    void SetupHomeAxesStateMachine();
    void SetupTwoSpotStateMachine();

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

    void move(QString axis, QString speed, QString position);
    void setSpeed(QString axis, QString speed);
    void setAbsMove(QString axis, QString position);

    LEDStatus m_ledStatus;

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

    SerialInterface *m_pSerialInterface;

    Axis m_Xaxis, m_Yaxis, m_Zaxis;
    Stage m_stage;

    QStringList m_axisStatus;

    bool m_sameStateXYZ;
    bool m_joystickOn;
    bool m_joyButtonOn;
    bool m_doorsOpen;
    bool m_vacOn;
    bool m_N2PurgeOn;

    // coordinate transforms
    double m_Xp2Base;
    double m_Yp2Base;
    double m_Zp2Base;
    double m_Xs2PH;
    double m_Ys2PH;
};

#endif // AXESCONTROLLER_H
