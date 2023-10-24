#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "plasmarecipe.h"
#include "axescontroller.h"
#include "commandmap.h"
#include "ledstatus.h"
#include "plasmahead.h"
#include "pwr.h"
#include "tuner.h"
#include "mfc.h"
#include "settingsdialog.h"
#include "lighttower.h"
#include "diameter.h"


#include <QObject>
#include <vector>
#include <memory>

class PlasmaController : public QObject
{
    Q_OBJECT

public:
    explicit PlasmaController(QWidget* parent = nullptr);
    ~PlasmaController();

    // Serial Functions
    bool open(const SettingsDialog& settings);
    void close();
    bool sendCommand(const QString& command);
    QString readResponse();
    QString getPortErrorString();
    bool isOpen();

    // Commands Map functions.  TODO: remove
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;

    // Poll Commands
    void getCTLStatusCommand();
    void parseResponseForCTLStatus(const QString &response);

    // CTL Status
    void setLEDStatus(int& bits);

    MFC* findMFCByNumber(int mfcNumber);
    int numberOfMFCs();
    void runDiameter();

    // startup and reset
    void CTLStartup();
    void resetCTL();

    // state machines
    void RunScanAxesSM();
    void RunCollisionSM();
    void StartScan() { emit SSM_TransitionStartup(); }
    void StopScan() { emit SSM_TransitionShutdown(); }

    // Recipe
    void setRecipe(QString filePath);
    bool getExecuteRecipe() const;
    void setExecuteRecipe(bool value);
    void processRecipeKeys();
    void setMFCsFlowFromRecipe();
    void setRFSetpointFromRecipe();
    void setTunerSetpointFromRecipe();

    // accessors
    Tuner& getTuner() { return m_tuner; }
    PWR& getPower() { return m_pwr; }
    QList<MFC*>& getMFCs() { return m_mfcs; }
    PlasmaHead& getPlasmaHead() { return m_plasmaHead; }
    SerialInterface* getSerialInterface() { return m_pSerialInterface; }
    PlasmaRecipe *getRecipe() { return m_pRecipe; }
    Diameter& getDiameter() { return m_waferDiameter; }
    AxesController& getAxesController() { return this->m_stageCTL; }

    // laser access
    void LaserSenseOn();
    void LaserSenseOff();
    void PollForCollision(); // sets the m_bCollisionDetected flag

    // query the controller
    void getFirmwareVersion();
    void howManyMFCs();
    void getBatchIDLogging();
    void getMFC4Range();
    void getMFC3Range();
    void getMFC2Range();
    void getMFC1Range();
    void getRecipeMBPosition();
    void getRecipeRFPosition();
    void getRecipeMFC4Flow();
    void getRecipeMFC3Flow();
    void getRecipeMFC2Flow();
    void getRecipeMFC1Flow(); 
    void getMaxRFPowerForward();
    void getAutoMan();
    void turnOffExecRecipe();
    void turnOnExecRecipe();
    void getPHSlitLength();
    void getPHSlitWidth();
    void getPHSafetyGap();
    void getTemp();

signals:
    void responseReceived(const QString& response);
    void executeRecipeChanged();
    void mainPortOpened();
    void setRecipeMBtuner(QString MBtunerSP);
    void setRecipeRFpower(QString RFpowerSP);

    // scan state machine transitions
    void SSM_TransitionStartup();
    void SSM_TransitionIdle();
    void SSM_TransitionShutdown();
    void SSM_TransitionRecycle();
    void SSM_TransitionParkZSubstate();
    void SSM_TransitionGoXYSubstate();
    void SSM_TransitionGoZPositionSubstate();
    void SSM_TransitionScanColSubstate();
    // scan state machine signals
    void SSM_Started();
    void SSM_Done();
    void SSM_StatusUpdate(QString status, QString next);

    // collision state machine transitions
    void CSM_TransitionStartup();
    void CSM_TransitionIdle();
    void CSM_TransitionShutdown();
    void CSM_TransitionGetZUp();
    void CSM_TransitionScanY();
    void CSM_TransitionGetZDown();
    // collision state machine status
    void CSM_StatusUpdate(QString status, QString next);

    void recipeExecutionStateChanged(bool state);

    void scanBoxChanged();

public slots:
    // MFCs
    int parseResponseForNumberOfMFCs(QString& responseStr);
    double handleGetMFCRecipeFlowCommand(QString& responseStr);
    void handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCDefaultRecipeCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCRangeCommand(const int mfcNumber, const double range);

    // Tuner
    void handleSetTunerDefaultRecipeCommand(const double defaultPosition);
    void handleSetTunerRecipePositionCommand(const int recipePosition);
    void handleSetTunerAutoTuneCommand(const bool value);

    // PWR
    void handleSetPWRDefaultRecipeCommand(const double defaultWatts);
    void handleSetPWRRecipeWattsCommand(const int recipeWatts);
    void handleSetPWRMaxWattsCommand(const double maxWatts);

    // light tower
    void handleLightTowerStateChange();
    void setLightTower();

    // scan state machine to idle
    void scanningStateMachineToIdle() { emit SSM_TransitionIdle(); }

    // axes
    void xLimitsChanged(double xmin, double xmax);
    void yLimitsChanged(double ymin, double ymax);

private:
    QString getLastCommand() { return m_pSerialInterface->getLastCommand(); }
    void setupScanStateMachine();
    void setupCollisionStateMachine();

    QList<MFC*> m_mfcs; // Store the MFCs in a list
    LightTower m_lightTower;
    SerialInterface *m_pSerialInterface;
    LEDStatus m_ledStatus;
    bool m_executeRecipe;
    CommandMap m_commandMap;
    AxesController m_stageCTL;
    Diameter m_waferDiameter;
    PlasmaRecipe *m_pRecipe;
    PlasmaHead m_plasmaHead;
    Tuner m_tuner;
    PWR m_pwr;

    // scan state machine
    QStateMachine m_scanStateMachine;
    QState *m_pScanSuperState;
    QState *m_pScanStartupState;
    QState *m_pScanState;
    QState *m_pParkZSubState;
    QState *m_pGoXYStartSubState;
    QState *m_pIdleSubState;
    QState *m_pGoZScanPositionSubState;
    QState *m_pScanColSubState;
    QState *m_pScanRecycleState;
    QState *m_pScanShutdownState;
    QState *m_pScanIdleState;

    // collision state machine
    QStateMachine m_collisionStateMachine;
    QState *m_pCPgetZUpstate;
    QState *m_pCPStartupState;
    QState *m_pCPScanYState;
    QState *m_pCPGetZDownState;
    QState *m_pCPIdleState;
    QState *m_pCPShutdownState;

    // laser state machine
    QStateMachine m_laserStateMachine;
    QState *m_pLaserTrippedstate;
    QState *m_pLaserActivateState;
    QState *m_pLaserDeactivateState;
    QState *m_pLaserIdleState;

    // base coordinates
    double m_scanMinXPos;
    double m_scanMaxXPos;
    double m_scanMinYPos;
    double m_scanMaxYPos;
    double m_scanZParkPos;
    double m_scanZScanPos;
    double m_scanRowXWidth;
    int m_numXRows;
    int m_currentXRow;
    int m_numCycles;
    int m_currentCycle;
    double m_startXPosition;
    double m_startYPosition;
    double m_scanYSpeed;
    double m_scanEndYPosition;

    int m_batchLogging;

    bool m_collisionDetected;
    bool m_collisionPassed;
    bool m_runRecipe; //Turn plasma on
    bool m_plannedAutoStart;
    bool m_plasmaActive;
};

#endif // PLASMACONTROLLER_H
