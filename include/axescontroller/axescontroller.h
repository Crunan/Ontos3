#ifndef AXESCONTROLLER_H
#define AXESCONTROLLER_H

#include <QObject>
#include <QStateMachine>
#include "settingsdialog.h"
#include "include/serialinterface.h"
#include "include/axescontroller/axis.h"
#include "include/stage.h"
#include "include/ledstatus.h"

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

    void StartInit();
    void RunInitAxesSM();
    void StartHome();
    void RunHomeAxesSM();

    void setXMaxPos(const double maxPos);
    void setXMaxSpeed(const double maxSpeed);
    void setXHomePos(const double homePos);

    void setYMaxPos(const double maxPos);
    void setYMaxSpeed(const double maxSpeed);
    void setYHomePos(const double homePos);

    void setZMaxPos(const double maxPos);
    void setZMaxSpeed(const double maxSpeed);
    void setZHomePos(const double homePos);

    void AxisStartup();

    QString getXMaxSpeedQStr() { return m_Xaxis.getMaxSpeedQStr(); }
    double XMaxSpeed() { return m_Xaxis.getMaxSpeed(); }
    QString getXHomePosQStr() { return m_Xaxis.getHomePosQStr(); }

    // command wrappers.  Maybe consider moving some to plasmacontroller
    void getXMaxSpeed();
    void getYMaxSpeed();
    void getZMaxSpeed();
    void getXp2Base();
    void getYp2Base();
    void getZp2Base() ;
    void getXs2PH();
    void getYs2PH();
    void getPHSlitLength();
    void getPHSlitWidth();
    void getPHSafetyZGap();
    void getZPinsBuried();
    void getZPinsExposed();
    void getLoadX2Base();
    void getLoadY2Base();
    void getLoadZ2Base();

    void setAxisStateMachinesIdle();

    void getAxisStatus();
    void updateAxisStatus();

    double getXPosition() const { return m_Xaxis.getPosition(); }
    double getYPosition() const { return m_Yaxis.getPosition(); }
    double getZPosition() const { return m_Zaxis.getPosition(); }

    void resetAxes();

    void togglePinsOn();
    void togglePinsOff();
    void toggleJoystickOn();
    void toggleJoystickOff();
    void toggleN2PurgeOn(Recipe &recipe);
    void toggleN2PurgeOff(Recipe &recipe);

signals:

    void stagePortOpened();
    void stageStatusUpdate(QString status1, QString status2);
    void stageResponseReceived(QString resonse);

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

    /*/ update ui
    void currentStatusChanged();
    void axisStatusChanged();
    void ledStatesChanged();
    void doorsOpenChanged();
    void joystickOnChanged();
    void vacuumOnChanged();
    void nitrogenPurgeOnChanged();*/

private slots:

private:
    // setup state machines
    void SetupInitAxesStateMachine();
    void SetupHomeAxesStateMachine();

    // utility functions
    bool nextStateReady();
    void setValve2(QString toggle);
    void stopMotors();
    void sendInitCMD();
    void setSameStateXYZsame();
    void parseStatus(QString serialResponse);

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
    QState *m_pHomeAxesStartupState;
    QState *m_pHomeAxesWaitParkZState;
    QState *m_pHomeAxesHomeXYState;
    QState *m_pHomeAxesWaitHomeXYState;
    QState *m_pHomeAxesHomeZState;
    QState *m_pHomeAxesWaitHomeZState;
    QState *m_pHomeAxesShutdownState;
    QState *m_pHomeAxesIdleState;

    SerialInterface m_serialInterface;

    Axis m_Xaxis, m_Yaxis, m_Zaxis;
    Stage m_stage;

    QStringList m_axisStatus;

    bool m_sameStateXYZ;
};

#endif // AXESCONTROLLER_H
