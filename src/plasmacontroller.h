#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "plasmarecipe.h"
#include "axescontroller.h"
#include "commandmap.h"
#include "configuration.h"
#include "ledstatus.h"
#include "plasmahead.h"
#include "pwr.h"
#include "tuner.h"
#include "mfc.h"
#include "settingsdialog.h"


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
    QString formatSerialCommand(QString cmd, const QString& setpoint);
    bool sendCommand(const QString& command);
    QString readResponse();

    QString getPortErrorString();
    bool isOpen();

    // Commands Map functions
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;

    // Poll Commands
    void getCTLStatusCommand();
    void parseResponseForCTLStatus(const QString &response);

    // CTL Status
    void setLEDStatus(int& bits);

    // MFC public commands
    // MFC functions
    MFC* findMFCByNumber(int mfcNumber);
    int numberOfMFCs();

    // Recipe functions
    bool getExecuteRecipe() const;
    void setExecuteRecipe(bool value);

    void CTLStartup();

    SerialInterface* getSerialInterface() { return m_pSerialInterface; }

    void RunScanAxesSM();
    void RunCollisionSM();

    // axes controller wrappers
    AxesController& getAxesController() { return this->m_stageCTL; }
    void AxisStartup() { m_stageCTL.AxisStartup(); }
    void resetAxes() { m_stageCTL.resetAxes(); }
    double getXTwoSpotFirstPoint() { return m_stageCTL.getXTwoSpotFirstPoint(); }
    double getXTwoSpotSecondPoint() { return m_stageCTL.getXTwoSpotSecondPoint(); }
    double getYTwoSpotFirstPoint() { return m_stageCTL.getYTwoSpotFirstPoint(); }
    double getYTwoSpotSecondPoint() { return m_stageCTL.getYTwoSpotSecondPoint(); }
    void RunInitAxesSM() {  m_stageCTL.RunInitAxesSM(); }
    void RunTwoSpotSM() {  m_stageCTL.RunTwoSpotSM(); }
    void RunHomeAxesSM() {  m_stageCTL.RunHomeAxesSM(); }
    void getAxisStatus() {  m_stageCTL.getAxisStatus(); }
    int getXAxisState() { return m_stageCTL.getXAxisState(); }
    double getXPosition() {  return m_stageCTL.getXPosition(); }
    double TranslateCoordXPH2Base(double Xpos) {  return m_stageCTL.TranslateCoordXPH2Base(Xpos); }
    int getYAxisState() { return m_stageCTL.getYAxisState(); }
    double getYPosition() { return m_stageCTL.getYPosition(); }
    double TranslateCoordYPH2Base(double Ypos) {  return m_stageCTL.TranslateCoordYPH2Base(Ypos); }
    int getZAxisState() { return m_stageCTL.getYAxisState(); }
    double getZPosition() { return m_stageCTL.getYPosition(); }
    double TranslateCoordZPH2Base(double Zpos) {  return m_stageCTL.TranslateCoordZPH2Base(Zpos); }
    QString getXMaxSpeedQStr() {  return m_stageCTL.getXMaxSpeedQStr(); }
    double XMaxSpeed() {  return m_stageCTL.XMaxSpeed(); }
    void StartInit() {  m_stageCTL.StartInit(); }
    void StartTwoSpot() {  m_stageCTL.StartTwoSpot(); }
    void StopTwoSpot() {  m_stageCTL.StopTwoSpot(); }
    void StartHome() {  m_stageCTL.StartHome(); }
    void StopHome() {  m_stageCTL.StopHome(); }
    void togglePinsOn() {  m_stageCTL.togglePinsOn(); }
    void togglePinsOff() {  m_stageCTL.togglePinsOff(); }
    void toggleN2PurgeOn() {  m_stageCTL.toggleN2PurgeOn(); }
    void toggleN2PurgeOff() {  m_stageCTL.toggleN2PurgeOff(); }
    void toggleJoystickOn() {  m_stageCTL.toggleJoystickOn(); }
    void toggleJoystickOff() {  m_stageCTL.toggleJoystickOff(); }
    void toggleVacOn() {  m_stageCTL.toggleVacOn(); }
    void toggleVacOff() {  m_stageCTL.toggleVacOff(); }

    // Recipe wrappers
    PlasmaRecipe *getRecipe() { return m_pRecipe; }

    void setRecipe(QString filePath) {
        m_pRecipe->fileReader.setFilePath(filePath);
        m_pRecipe->setRecipeFromFile();
    }

    QMap<QString, QVariant> getRecipeMap() { return m_pRecipe->getRecipeMap(); }
    void addRecipeToCascade(QString fileName) { m_pRecipe->addRecipeToCascade(fileName); }
    void removeRecipeFromCascade(QString fileName) { m_pRecipe->removeRecipeFromCascade(fileName); }
    QList<QString>  getCascadeRecipeList() { return m_pRecipe->getCascadeRecipeList(); }
    void setRecipeThickness(double thickness) { m_pRecipe->setThickness(thickness); }
    QString getRecipeThicknessQStr() const { return m_pRecipe->getThicknessQStr(); }
    void setRecipeGap(double gap) { m_pRecipe->setGap(gap); }
    QString getRecipeGapQStr() const { return m_pRecipe->getGapQStr(); }
    void setRecipeOverlap(double overlap) { m_pRecipe->setOverlap(overlap); }
    QString getRecipeOverlapQStr() const { return m_pRecipe->getOverlapQStr(); }
    void setRecipeSpeed(double speed) { m_pRecipe->setSpeed(speed); }
    QString getRecipeSpeedQStr() const { return m_pRecipe->getSpeedQStr(); }
    void setRecipeCycles(int cycles) { m_pRecipe->setCycles(cycles); }
    QString getRecipeCyclesQStr() const { return m_pRecipe->getCyclesQStr(); }

    void StartScan() { emit SSM_TransitionStartup(); }
    void StopScan() { emit SSM_TransitionShutdown(); }

    void LaserSenseOn();
    void LaserSenseOff();
    void PollForCollision(); // sets the m_bCollisionDetected flag

    // query the controller
    void howManyMFCs();
    void getBatchIDLogging();
    void getRecipeMBPosition();
    void getRecipeRFPosition();
    void getRecipeMFC4Flow();
    void getRecipeMFC3Flow();
    void getRecipeMFC2Flow();
    void getRecipeMFC1Flow();
    void getMFC4Range();
    void getMFC3Range();
    void getMFC2Range();
    void getMFC1Range();
    void getMaxRFPowerForward();
    void getAutoMan();
    void getTemp();
    void turnOffExecRecipe();
    void getPHSlitLength();
    void getPHSlitWidth();

    Tuner m_tuner;
    PlasmaHead m_plasmaHead;
    PWR m_pwr;
    QList<MFC*> m_mfcs; // Store the MFCs in a list

signals:
    void responseReceived(const QString& response);
    void executeRecipeChanged();
    void mainPortOpened();
    void setRecipeMBtuner(QString MBtunerSP);
    void setRecipeRFpower(QString RFpowerSP);
    void MFC4RecipeFlow(QString recipeFlow);
    void MFC3RecipeFlow(QString recipeFlow);
    void MFC2RecipeFlow(QString recipeFlow);
    void MFC1RecipeFlow(QString recipeFlow);
    void plasmaHeadTemp(double m_headTemp);

    // scan state machine transitions
    void SSM_TransitionStartup();
    void SSM_TransitionIdle();
    void SSM_TransitionShutdown();
    void SSM_TransitionRecycle();
    void SSM_TransitionParkZSubstate();
    void SSM_TransitionGoXYSubstate();
    void SSM_TransitionGoZPositionSubstate();
    void SSM_TransitionScanColSubstate();

    // collision state machine transitions
    void CSM_TransitionStartup();
    void CSM_TransitionIdle();
    void CSM_TransitionShutdown();
    void CSM_TransitionGetZUp();
    void CSM_TransitionScanY();
    void CSM_TransitionGetZDown();

public slots:
    // MFCs
    int parseResponseForNumberOfMFCs(QString& responseStr);
    double handleGetMFCRecipeFlowCommand(QString& responseStr);

    void handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCDefaultRecipeCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCRangeCommand(const int mfcNumber, const double range);

    // Tuner
    void handleSetTunerDefaultRecipeCommand(const double defaultPosition);
    void handleSetTunerRecipePositionCommand(const double recipePosition);
    void handleSetTunerAutoTuneCommand(const bool value);

    // PWR
    void handleSetPWRDefaultRecipeCommand(const double defaultWatts);
    void handleSetPWRRecipeWattsCommand(const double recipeWatts);
    void handleSetPWRMaxWattsCommand(const double maxWatts);

private:
    QString getLastCommand() { return m_pSerialInterface->getLastCommand(); }
    void setupScanStateMachine();
    void setupCollisionStateMachine();

    SerialInterface *m_pSerialInterface;
    LEDStatus m_ledStatus;
    bool m_executeRecipe;
    Configuration m_config;
    CommandMap m_commandMap;
    AxesController m_stageCTL;
    PlasmaRecipe *m_pRecipe;

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

    double m_scanMinXPos;
    double m_scanMaxXPos;
    double m_scanMinYPos;
    double m_scanMaxYPos;
    double m_scanZParkPos;
    double m_scanZScanPos;
    double m_scanRowXWidth;
    double m_PHSlitLength;
    double m_PHSlitWidth;
    int m_numXRows;
    int m_currentXRow;
    int m_numCycles;
    int m_currentCycle;
    double m_startXPosition;
    double m_startYPosition;
    double m_scanYSpeed;
    double m_scanEndYPosition;

    double m_XMaxPos;
    double m_XMinPos;
    double m_YMaxPos;
    double m_YMinPos;

    int m_numMFCs;
    int m_batchLogging;
    bool m_readyToLoad;
    double m_MBtunerRecipeSP;
    double m_RFtunerRecipeSP;
    int m_MaxRFPowerForward;
    int m_autoTune;
    double m_headTemp;

    bool m_bCollisionDetected;
};

#endif // PLASMACONTROLLER_H
