#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include "UtilitiesAndConstants.h"

int SM_PollCounter = 0;
const int SM_POLL_PERIOD = 5;
std::chrono::milliseconds CTLResetTimeOut = 0ms;

MainWindow::MainWindow(MainLoop* loop, QWidget *parent) :
    QMainWindow(parent),
    m_pMainLoop(loop),
    ui(new Ui::MainWindow),
    m_pStatus(new QLabel),
    m_passDialog(this),
    m_pSettings(new SettingsDialog),
    m_mainCTL(),
    m_persistentSettings(),
    m_pRecipeFileDialog(0), // gets initialized at time of use
    m_pRecipeInputDialog(new QInputDialog(this)),
    m_config(),
    m_engineeringMode(false)
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE v" + QString(APP_VERSION));

    // setup the state machine
    setupMainStateMachine();

    // ui updates from axescontroller
    connect(&m_mainCTL.getAxesController(), &AxesController::stageStatusUpdate, this, &MainWindow::stageStatusUpdate);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMStartup, this, &MainWindow::homeStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMDone, this, &MainWindow::homeStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::initSMStartup, this, &MainWindow::initStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::initSMDone, this, &MainWindow::initStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::pinsStateChanged, this, &MainWindow::pinsStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::joystickStateChanged, this, &MainWindow::joystickStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::n2StateChanged, this, &MainWindow::n2StateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::vacStateChanged, this, &MainWindow::vacStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::updateUIAxisStatus, this, &MainWindow::AxisStatusToUI);
    // ui updates from plasma controller and sub objects
    connect(&m_mainCTL, &PlasmaController::SSM_StatusUpdate, this, &MainWindow::SSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::SSM_Started, this, &MainWindow::SSM_Started);
    connect(&m_mainCTL, &PlasmaController::SSM_Done, this, &MainWindow::SSM_Done);
    connect(&m_mainCTL, &PlasmaController::CSM_StatusUpdate, this, &MainWindow::CSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::scanBoxChanged, this, &MainWindow::scanBoxChanged);
    connect(&m_mainCTL, &PlasmaController::plasmaStateChanged, this, &MainWindow::plasmaStateChanged);
    connect(&m_mainCTL, &PlasmaController::batchIDLoggingIsActive, this, &MainWindow::batchIDLoggingIsActive);
    connect(&m_mainCTL, &PlasmaController::setUINumberOfMFCs, this, &MainWindow::setUINumberOfMFCs);
    connect(&m_mainCTL.getTuner(), &Tuner::recipePositionChanged, this, &MainWindow::setRecipeMBtuner);
    connect(&m_mainCTL.getTuner(), &Tuner::updateUIRecipePosition, this, &MainWindow::setRecipeMBtuner);
    connect(&m_mainCTL.getPlasmaHead(), &PlasmaHead::headTemperatureChanged, this, &MainWindow::headTemperatureChanged);
    connect(&m_passDialog, &PasswordDialog::userEnteredPassword, this, &MainWindow::userEnteredPassword);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::serialClosed, this, &MainWindow::serialDisconnected);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::serialOpen, this, &MainWindow::serialConnected);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::readTimeoutError, this, &MainWindow::readTimeoutError);
    connect(&m_mainCTL.getAbortMessages(), &AbortCodeMessages::showAbortMessageBox, this, &MainWindow::showAbortMessageBox);
    // run the next cascade recipe
    connect(&m_mainCTL, &PlasmaController::loadCascadeRecipe, this, &MainWindow::loadCascadeRecipe);
    // main state machine from main loop
    connect(m_pMainLoop, &MainLoop::runMainStateMachine, this, &MainWindow::runMainStateMachine);
    // ui updates from recipe
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::thicknessChanged, this, &MainWindow::thicknessChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::gapChanged, this, &MainWindow::gapChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::overlapChanged, this, &MainWindow::overlapChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::speedChanged, this, &MainWindow::speedChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::autoScanChanged, this, &MainWindow::autoScanChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::xLimitsChanged, this, &MainWindow::xLimitsChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::yLimitsChanged, this, &MainWindow::yLimitsChanged);
    connect(m_mainCTL.getRecipe(), &PlasmaRecipe::cyclesChanged, this, &MainWindow::cyclesChanged);
    connect(&m_mainCTL.getPower(), &PWR::recipeWattsChanged, this, &MainWindow::recipeWattsChanged);
    connect(&m_mainCTL.getPower(), &PWR::updateUIRecipeWatts, this, &MainWindow::recipeWattsChanged);
    connect(&m_mainCTL.getPower(), &PWR::forwardWattsChanged, this, &MainWindow::forwardWattsChanged);
    connect(&m_mainCTL.getPower(), &PWR::reflectedWattsChanged, this, &MainWindow::reflectedWattsChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::autoTuneChanged, this, &MainWindow::autoTuneChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::updateUIAutoTune, this, &MainWindow::autoTuneChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::actualPositionChanged, this, &MainWindow::MBactualPositionChanged);

    // disable until implemented
    ui->request_terminal->setEnabled(false);

    // Make signal/slot connections here
    connectRecipeButtons();

    // MFC slots
    connectMFCFlowBars();

    // MFC labels
    setMFCLabels();

    // status bar
    ui->statusBar->addWidget(m_pStatus);

    // setup wafer diamter combo box
    ui->wafer_diameter->addItems(m_mainCTL.getDiameter().getWaferDiameterTextList());

    // Setup Recipe List for Cascade Recipes
    populateRecipeListWidgetFromDirectory(ui->listRecipes);

    // give things a little time to settle before opening the serial port
    QTimer::singleShot(50, this, &MainWindow::openMainPort);
}

MainWindow::~MainWindow() {
    delete m_pSettings;
    delete ui;
    delete m_pRecipeInputDialog;

    // cleanup state machine
    delete m_pMainStartupState;
    delete m_pMainIdleState;
    delete m_pMainPollingState;
    delete m_pMainShutdownState;
    delete m_pMainLoop;
}

// show status message at the bottom on MainWindow
void MainWindow::showStatusMessage(const QString &message)
{
    m_pStatus->setText(message);
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    Logger::logInfo("Shutting Down -------------------");
    Logger::clean();

    m_pMainLoop->stop();
}

// style sheets take a bit to load when starting up.
// the result is that the style sheets don't take full effect
// until after the mainwindow is visible.  This creates a little
// delay to handle that
void MainWindow::showEvent(QShowEvent *)
{
    // hide stage controls
    showStageControls(false);

    QTimer::singleShot(50, this, &MainWindow::setInitialUIState);
}

void MainWindow::setInitialUIState()
{
    setUIOperatorMode();
    // show/hide button and text box
    batchIDEnabled();
    // set default recipe params
    ui->thickness_recipe->setText(m_mainCTL.getRecipe()->getThicknessQStr());
    ui->gap_recipe->setText(m_mainCTL.getRecipe()->getGapQStr());
    ui->overlap_recipe->setText(m_mainCTL.getRecipe()->getOverlapQStr());
    ui->speed_recipe->setText(m_mainCTL.getRecipe()->getSpeedQStr());
    ui->cycles_recipe->setText(m_mainCTL.getRecipe()->getCyclesQStr());
    ui->x1_recipe->setText(m_mainCTL.getRecipe()->getXminPHQStr());
    ui->x2_recipe->setText(m_mainCTL.getRecipe()->getXmaxPHQStr());
    ui->y1_recipe->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->y2_recipe->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());
}

void MainWindow::showStageControls(bool show)
{
    if (show) {
        ui->stageControlsWidget->show();
        ui->stageStatusWidget->show();
    }
    else {
        ui->stageControlsWidget->hide();
        ui->stageStatusWidget->hide();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Startup/ Setup
//////////////////////////////////////////////////////////////////////////////////

void MainWindow::readSettings()
{
    // read the persistent settings
    bool batchID = m_persistentSettings.value(BATCHID_ENABLED_SETTING, false).toBool();
    bool collisionSystem = m_persistentSettings.value(COLLISION_SYSTEM_ENABLED_SETTING, false).toBool();

    // update the UI components
    if (batchID) {
        m_mainCTL.batchIDLoggingOn(true);
        batchIDLoggingIsActive();
    }
    ui->collision_system_checkbox->setChecked(collisionSystem);
}

void MainWindow::connectRecipeButtons()
{
    //MFC buttons
    connectMFCRecipeButton(ui->loadMFC1Button, 1);
    connectMFCRecipeButton(ui->loadMFC2Button, 2);
    connectMFCRecipeButton(ui->loadMFC3Button, 3);
    connectMFCRecipeButton(ui->loadMFC4Button, 4);
    connectMFCRecipeButton(ui->loadMFC5Button, 5);
    connectMFCRecipeButton(ui->loadMFC6Button, 6);
}

void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < m_mainCTL.getMFCs().size(); ++i) {
        connect(m_mainCTL.getMFCs()[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeFlow);
        connect(m_mainCTL.getMFCs()[i], &MFC::updateUIRecipeFlow, this, &MainWindow::updateRecipeFlow);
        connect(m_mainCTL.getMFCs()[i], &MFC::updateUIActualFlow, this, &MainWindow::actualFlowChanged);
        connect(m_mainCTL.getMFCs()[i], &MFC::rangeChanged, this, &MainWindow::rangeChanged);
    }
}

void MainWindow::setMFCLabels()
{
    QString MFC1_label = m_config.getValueForKey(CONFIG_MFC1_LABEL_KEY);
    QString MFC2_label = m_config.getValueForKey(CONFIG_MFC2_LABEL_KEY);
    QString MFC3_label = m_config.getValueForKey(CONFIG_MFC3_LABEL_KEY);
    QString MFC4_label = m_config.getValueForKey(CONFIG_MFC4_LABEL_KEY);
    QString MFC5_label = m_config.getValueForKey(CONFIG_MFC5_LABEL_KEY);
    QString MFC6_label = m_config.getValueForKey(CONFIG_MFC6_LABEL_KEY);

    // MFC1
    if (MFC1_label != QString()) {
        ui->gas1_label->setText(MFC1_label);
    }
    // MFC2
    if (MFC2_label != QString()) {
        ui->gas2_label->setText(MFC2_label);
    }
    // MFC3
    if (MFC3_label != QString()) {
        ui->gas3_label->setText(MFC3_label);
    }
    // MFC4
    if (MFC4_label != QString()) {
        ui->gas4_label->setText(MFC4_label);
    }
    // MFC5
    if (MFC5_label != QString()) {
        ui->gas5_label->setText(MFC5_label);
    }
    // MFC6
    if (MFC6_label != QString()) {
        ui->gas6_label->setText(MFC6_label);
    }
}

void MainWindow::connectMFCRecipeButton(QPushButton* button, const int& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}

// determine if there is a 3 axis board attached
void MainWindow::has3Axis()
{
    m_has3AxisBoard = m_mainCTL.has3Axis();
}

//////////////////////////////////////////////////////////////////////////////////
// State machine slots
//////////////////////////////////////////////////////////////////////////////////

// set ui elements accordingly
void MainWindow::homeStateMachineStartup()
{
    ui->Home_button->setText("STOP");

    // disable other stage movement buttons
    ui->init_button->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->Stagepins_button->setEnabled(false);
    ui->vac_button->setEnabled(false);
}

// set ui elements accordingly
void MainWindow::homeStateMachineDone()
{
    ui->Home_button->setChecked(false);
    ui->Home_button->setText("LOAD");

    ui->scan_button->setEnabled(true);
    ui->init_button->setEnabled(true);
    ui->Stagepins_button->setEnabled(true);
    ui->vac_button->setEnabled(true);

    if (m_engineeringMode) {
        // enable other stage movement buttons
        ui->diameter_button->setEnabled(true);
    }
}

// set ui elements accordingly
void MainWindow::initStateMachineStartup()
{
    // disaable the init buttons
    ui->init_button->setEnabled(false);

    // disable the other state movement buttons
    ui->diameter_button->setEnabled(false);
    ui->wafer_diameter->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->menuStage_Test->setEnabled(false);
}

// set ui elements accordingly
void MainWindow::initStateMachineDone()
{
    // enable the init buttons again
    ui->init_button->setEnabled(true);
    ui->scan_button->setEnabled(true);
    ui->Home_button->setEnabled(true);
    ui->vac_button->setEnabled(true);
    ui->Stagepins_button->setEnabled(true);

    if (m_engineeringMode) {
        // enable the buttons that we disabled
        ui->diameter_button->setEnabled(true);
        ui->wafer_diameter->setEnabled(true);
        ui->diameter_button->setEnabled(true);
        ui->menuStage_Test->setEnabled(true);
        ui->Joystick_button->setEnabled(true);
    }
}

void MainWindow::SSM_Started()
{
    ui->scan_button->setText("STOP");

    // disable other stage movement buttons
    ui->init_button->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->Stagepins_button->setEnabled(false);
    ui->vac_button->setEnabled(false);
}

void MainWindow::SSM_Done()
{
    ui->scan_button->setChecked(false);
    ui->scan_button->setText("SCAN");

    // update status
    ui->axisstatus->setText("Scanning Completed");
    ui->axisstatus_2->setText("");

    // reenable
    ui->init_button->setEnabled(true);
    ui->Home_button->setEnabled(true);
    ui->scan_button->setEnabled(true);
    ui->vac_button->setEnabled(true);
    ui->Stagepins_button->setEnabled(true);
}

void MainWindow::scanBoxChanged()
{   
    // dashboard tab
    ui->x1_recipe->setText(m_mainCTL.getRecipe()->getXminPHQStr());
    ui->x2_recipe->setText(m_mainCTL.getRecipe()->getXmaxPHQStr());
    ui->y1_recipe->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->y2_recipe->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());
}

//////////////////////////////////////////////////////////////////////////////////
// Serial Ports
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::closeMainPort()
{
    if (m_mainCTL.isOpen()) {
        m_mainCTL.close();
    }
}

void MainWindow::openMainPort()
{
    QString configPort = m_config.getValueForKey(CONFIG_COM_PORT_KEY);

    // use value from config file if it exists
    if (configPort != "")
        m_pSettings->setName(configPort);

    SettingsDialog::Settings p = m_pSettings->settings();

    if (!m_mainCTL.open(*m_pSettings)) {

        QMessageBox::critical(this, "Error:  ", m_mainCTL.getPortErrorString());
        showStatusMessage("Open error:  " + m_mainCTL.getPortErrorString());
    }
}

void MainWindow::readTimeoutError(QString lastCommand)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Error: ", "Read timeout on command " + lastCommand + ". Exit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::serialDisconnected()
{
    // Update Status bar
    showStatusMessage("Disconnected");

    disableControlButtons();

    // stop the main state machine
    emit MSM_TransitionShutdown();
}

void MainWindow::serialConnected()
{
    // determine if there is a 3 axis board in the system as soon as possible
    has3Axis();

    // show stage controls if 3 axis board is connected
    showStageControls(m_has3AxisBoard);

    if (m_has3AxisBoard) {
        m_mainCTL.getAxesController().resetAxes(); // reset axes controller if one is connected
    }
    m_mainCTL.resetCTL();

    // this handles the edge case where the board is not present
    // but the serial port is successuflly opened.  The above
    // serial commands will fail and close the port so stop here.
    if (!m_mainCTL.isOpen()) return;

    CTLResetTimeOut = 2500ms / m_pMainLoop->getTimerInterval();

    // start the main state machine
    emit MSM_TransitionStartup();

    // allow user to init only
    ui->init_button->setEnabled(true);
    if (m_engineeringMode) {
        ui->n2_purge_button->setEnabled(true);
        ui->plsmaBtn->setEnabled(true);
    }

    // clear any remaing status statements
    ui->axisstatus->clear();
    ui->axisstatus_2->clear();

    SettingsDialog::Settings p = m_pSettings->settings();

    // Give status on connect
    showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name, p.stringBaudRate, p.stringDataBits, p.stringParity, p.stringStopBits, p.stringFlowControl));
}

//////////////////////////////////////////////////////////////////////////////////
// State machine and execution
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupMainStateMachine()
{
    m_pMainStartupState = new QState();
    m_pMainIdleState = new QState();
    m_pMainPollingState = new QState();
    m_pMainShutdownState = new QState();

    // construct operating transitions
    m_pMainStartupState->addTransition(this, &MainWindow::MSM_TransitionPolling, m_pMainPollingState);
    m_pMainIdleState->addTransition(this, &MainWindow::MSM_TransitionStartup, m_pMainStartupState);

    // shutdown transitions
    m_pMainStartupState->addTransition(this, &MainWindow::MSM_TransitionShutdown, m_pMainShutdownState);
    m_pMainPollingState->addTransition(this, &MainWindow::MSM_TransitionShutdown, m_pMainShutdownState);
    m_pMainIdleState->addTransition(this, &MainWindow::MSM_TransitionShutdown, m_pMainShutdownState);

    // idle transitions
    m_pMainStartupState->addTransition(this, &MainWindow::MSM_TransitionIdle, m_pMainIdleState);
    m_pMainPollingState->addTransition(this, &MainWindow::MSM_TransitionIdle, m_pMainIdleState);
    m_pMainShutdownState->addTransition(this, &MainWindow::MSM_TransitionIdle, m_pMainIdleState);

    // add states to the machine
    m_mainStateMachine.addState(m_pMainIdleState);
    m_mainStateMachine.addState(m_pMainStartupState);
    m_mainStateMachine.addState(m_pMainPollingState);
    m_mainStateMachine.addState(m_pMainShutdownState);

    // set initial state to idle
    m_mainStateMachine.setInitialState(m_pMainIdleState);

    // start the state machine
    m_mainStateMachine.start();
}

void MainWindow::runMainStateMachine()
{
    if (m_mainStateMachine.configuration().contains(m_pMainStartupState)) { // in Startup state        
        if (CTLResetTimeOut > 0ms) {
            CTLResetTimeOut -= 1ms;
        }
        else {
            Logger::logInfo("Main State Machine Start Up");
            RunStartup();
            // transition to polling state
            emit MSM_TransitionPolling();
        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainPollingState)) { // in Polling state
        SM_PollCounter += 1;
        if (SM_PollCounter >= SM_POLL_PERIOD) {
            SM_PollCounter = 0;

            m_mainCTL.getCTLStatusCommand();

            if (m_has3AxisBoard) { // only do the below if a 3axis board is connected
                m_mainCTL.handleAutoScan();
                m_mainCTL.setLightTower();
                m_mainCTL.RunDoorOpenSM();
                m_mainCTL.getAxesController().RunInitAxesSM();
                m_mainCTL.getAxesController().RunStageTestSM();
                m_mainCTL.getAxesController().RunHomeAxesSM();
                m_mainCTL.RunScanAxesSM();
                m_mainCTL.RunCollisionSM();
                m_mainCTL.PollForCollision();
                m_mainCTL.getAxesController().getAxisStatus();
            }
        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainIdleState)) { // in Idle state
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainShutdownState)) { // in Shutdown state
        Logger::logInfo("Main State Machine Shut Down");

        emit MSM_TransitionIdle();
    }
}

void MainWindow::RunStartup()
{
    // read persistent settings and update UI
    // do this here since the settings could trigger
    // a command to be sent so we need the know the
    // serial port is ready to go
    readSettings();

    m_mainCTL.CTLStartup(m_has3AxisBoard);

    if (m_has3AxisBoard) {
        m_mainCTL.getAxesController().AxisStartup();
    }

    // handshake
    bool ok = false;
    int handshake = m_config.getValueForKey(HANDSHAKE).toInt(&ok);
    if (ok) {
        if (handshake == 1) {
            m_mainCTL.handshakeOn(true);
        }
        else {
            m_mainCTL.handshakeOn(false);
        }
    }
    else {
        Logger::logCritical("Cannot find config file entry for: " + QString(HANDSHAKE));
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Plasma controller
//////////////////////////////////////////////////////////////////////////////////

void MainWindow::setRecipeMBtuner(double MBtunerSP)
{
    ui->mb_recipe->setText(QString::number(MBtunerSP, 'f', 2));
}

void MainWindow::CSM_StatusUpdate(QString status, QString next)
{
    ui->axisstatus->setText(status);
    ui->axisstatus_2->setText(next);
}

void MainWindow::SSM_StatusUpdate(QString status, QString next)
{
    ui->axisstatus->setText(status);
    ui->axisstatus_2->setText(next);
}

void MainWindow::showAbortMessageBox(QString message, bool shutdown)
{
    if (shutdown) {
        QMessageBox::critical(this, "ABORT CONDITION", message + " Press OK to shutdown app");
        QApplication::quit();
    }
    else {
        QMessageBox::critical(this, "ABORT CONDITION", message);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// 3 axis
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::pinsStateChanged(bool state)
{
    if (state) {
        ui->Stagepins_button->setText("PINS OFF");
    }
    else {
        ui->Stagepins_button->setText("PINS");
    }
}

void MainWindow::joystickStateChanged(bool state)
{
    if (state) {
        ui->Joystick_button->setText("JOY OFF");
    }
    else {
        ui->Joystick_button->setText("JOY");
    }
}
void MainWindow::n2StateChanged(bool state)
{
    if (state) {
        ui->n2_purge_button->setText("N2 OFF");
    }
    else {
        ui->n2_purge_button->setText("N2 PURGE");
    }
}
void MainWindow::vacStateChanged(bool state)
{
    if (state) {
        ui->vac_button->setText("VAC OFF");
    }
    else {
        ui->vac_button->setText("VAC");
    }
}

void MainWindow::stageStatusUpdate(QString statusNow, QString statusNext)
{
    // dashboard
    ui->axisstatus->setText(statusNow);
    ui->axisstatus_2->setText(statusNext);
}

void MainWindow::AxisStatusToUI()
{
    // XAxis
    if (m_mainCTL.getAxesController().getXAxisState() >= AXIS_IDLE) {
        double Xpos = m_mainCTL.getAxesController().getXPosition();
        ui->X_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordXBase2PH(Xpos)));
    }
    else {
        ui->X_relative_PH->setText("???");
    }
    // YAxis
    if (m_mainCTL.getAxesController().getYAxisState() >= AXIS_IDLE) {
        double Ypos = m_mainCTL.getAxesController().getYPosition();
        ui->Y_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordYBase2PH(Ypos)));
    }
    else {
        ui->Y_relative_PH->setText("???");
    }
    // ZAxis
    if (m_mainCTL.getAxesController().getZAxisState() >= AXIS_IDLE) {
        double Zpos = m_mainCTL.getAxesController().getZPosition();
        ui->Z_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordZBase2PH(Zpos)));
    }
    else {
        ui->Z_relative_PH->setText("???");
    }
}


//////////////////////////////////////////////////////////////////////////////////
// Recipe
//////////////////////////////////////////////////////////////////////////////////

void MainWindow::openRecipe()
{
    // Create a file dialog
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);

    QString initialDirectory = RECIPE_DIRECTORY;

    m_pRecipeFileDialog = new QFileDialog(this, "Open Recipe File", initialDirectory, "Recipe Files (*.rcp)");
    m_pRecipeFileDialog->setAcceptMode(QFileDialog::AcceptOpen);

    connect(m_pRecipeFileDialog, &QFileDialog::fileSelected, this, &MainWindow::OpenRecipeFileSelected);
    connect(m_pRecipeFileDialog, &QFileDialog::rejected, this, &MainWindow::OpenRecipeRejected);

    m_pRecipeFileDialog->setOption(QFileDialog::DontUseNativeDialog);
    m_pRecipeFileDialog->show();
}


void MainWindow::OpenRecipeFileSelected(const QString &file)
{
    // set appropriate fields to use the recipe
    installRecipe(file);

    // clear cascade recipe since it's now assumed the user does not want to run
    // cascade recipes
    ui->cascade_recipe_name->clear();
    ui->listCascadeRecipes->clear();
    m_mainCTL.clearCascadeRecipes();

    //delete m_pRecipeFileDialog;
}

void MainWindow::OpenRecipeRejected()
{
   delete m_pRecipeFileDialog;
}

void MainWindow::installRecipe(QString sRecipeFileAndPath)
{
    // set the filename
    QFileInfo fi(sRecipeFileAndPath);
    ui->name_recipe->setText(fi.baseName());

    // set recipe path and file
    m_mainCTL.setRecipe(sRecipeFileAndPath);

    // enable the plasma button now that a recipe is loaded
    ui->plsmaBtn->setEnabled(true);

    Logger::logInfo("Recipe opened: " + sRecipeFileAndPath);
}

void MainWindow::saveRecipe()
{
    // Create the directory pathmfc4_recipe
    QString directoryPath = RECIPE_DIRECTORY;

    // Create the directory if it doesn't exist
    QDir directory;
    if (!directory.exists(directoryPath)) {
       directory.mkpath(directoryPath);
    }

    m_pRecipeFileDialog = new QFileDialog(this, "Save Recipe", directoryPath, "Recipe Files (*.rcp)");
    m_pRecipeFileDialog->setAcceptMode(QFileDialog::AcceptSave);

    connect(m_pRecipeFileDialog, &QFileDialog::fileSelected, this, &MainWindow::SaveRecipeFileSelected);
    connect(m_pRecipeFileDialog, &QFileDialog::rejected, this, &MainWindow::SaveRecipeFileRejected);

    m_pRecipeFileDialog->setOption(QFileDialog::DontUseNativeDialog);
    m_pRecipeFileDialog->show();
}

void MainWindow::SaveRecipeFileSelected(const QString &file)
{
    QString selectedFileName = file;
    if (!selectedFileName.isEmpty()) {
       // Create the file path
       QString filePath = selectedFileName;

       // determine is we need to add the file extension
       QFileInfo fi(filePath);
       if (fi.completeSuffix() != "rcp")
            filePath += ".rcp";

       // Open the file for writing
       QFile file(filePath);
       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            QMap<QString, QVariant> recipe = m_mainCTL.getRecipe()->getRecipeMap();
            // Write each recipe name to the file
            for (auto it = recipe.begin(); it != recipe.end(); it++) {
                const QString& key = it.key();
                QVariant value;

                if (key == RECIPE_MFC1_KEY)
                    value = m_mainCTL.getMFCs()[0]->getRecipeFlowQStr();
                else if (key == RECIPE_MFC2_KEY)
                    value = m_mainCTL.getMFCs()[1]->getRecipeFlowQStr();
                else if (key == RECIPE_MFC3_KEY)
                    value = m_mainCTL.getMFCs()[2]->getRecipeFlowQStr();
                else if (key == RECIPE_MFC4_KEY)
                    value = m_mainCTL.getMFCs()[3]->getRecipeFlowQStr();
                else if (key == RECIPE_MFC5_KEY)
                    value = m_mainCTL.getMFCs()[4]->getRecipeFlowQStr();
                else if (key == RECIPE_MFC6_KEY)
                    value = m_mainCTL.getMFCs()[5]->getRecipeFlowQStr();
                else if (key == RECIPE_PWR_KEY)
                    value = m_mainCTL.getPower().getRecipeWattsQStr();
                else if (key == RECIPE_TUNER_KEY)
                    value = m_mainCTL.getTuner().getRecipePositionQStr();
                else if (key == RECIPE_THICKNESS_KEY)
                    value = m_mainCTL.getRecipe()->getThicknessQStr();
                else if (key == RECIPE_GAP_KEY)
                    value = m_mainCTL.getRecipe()->getGapQStr();
                else if (key == RECIPE_OVERLAP_KEY)
                    value = m_mainCTL.getRecipe()->getOverlapQStr();
                else if (key == RECIPE_SPEED_KEY)
                    value = m_mainCTL.getRecipe()->getSpeedQStr();
                else if (key == RECIPE_CYCLES_KEY)
                    value = m_mainCTL.getRecipe()->getCyclesQStr();
                else if (key == RECIPE_XMIN_KEY)
                    value = m_mainCTL.getRecipe()->getXminPHQStr();
                else if (key == RECIPE_YMIN_KEY)
                    value = m_mainCTL.getRecipe()->getYminPHQStr();
                else if (key == RECIPE_XMAX_KEY)
                    value = m_mainCTL.getRecipe()->getXmaxPHQStr();
                else if (key == RECIPE_YMAX_KEY)
                    value = m_mainCTL.getRecipe()->getYmaxPHQStr();
                else if (key == RECIPE_PURGE_KEY)
                    value = m_mainCTL.getRecipe()->getPurgeQStr();
                else if (key == RECIPE_AUTOSCAN_KEY)
                    value = m_mainCTL.getRecipe()->getAutoScanQStr();
                else if (key == RECIPE_HEATER_KEY)
                    value = m_mainCTL.getRecipe()->getHeaterQStr();
                else
                    Logger::logCritical("Encountered unknown recipe key when saving recipe: " + key);

                out << "<" << key << ">" << value.toString() << "\n";
            }

            // update the recipe field
            ui->name_recipe->setText(fi.baseName());

            file.close();

            Logger::logInfo("Recipe saved to file: " + filePath);
       } else {
            Logger::logInfo("Failed to open file for writing: " + file.errorString());
       }
    }
    //delete m_pRecipeFileDialog;
}

void MainWindow::SaveRecipeFileRejected()
{
    delete m_pRecipeFileDialog;
}


// update the recipe progress bar and values
void MainWindow::updateRecipeFlow(const int mfcNumber, const double recipeFlow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(1)->getRange();       
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas1_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc1_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas1_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
    else if (mfcNumber == 2) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas2_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc2_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas2_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
    else if (mfcNumber == 3) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas3_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc3_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas3_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
    else if (mfcNumber == 4) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas4_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc4_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas4_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
    else if (mfcNumber == 5) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(5)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas5_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc5_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas5_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
    else if (mfcNumber == 6) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(6)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas6_sliderBar->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc6_recipe->setText(QString::number(recipeFlow, 'f', 2));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas6_recipe_SLPM->setText(QString::number(recipeFlow, 'f', 2));
    }
}

// update the recipe watts
void MainWindow::recipeWattsChanged()
{
    int watts = m_mainCTL.getPower().getRecipeWatts();
    ui->RF_recipe_watts->setText(QString::number(watts));
}

// update the recipe auto tune
void MainWindow::autoTuneChanged(bool autoTune)
{
    (autoTune) ? ui->autotune_recipe->setText("ON") : ui->autotune_recipe->setText("OFF");
}

// update recipe thickness
void MainWindow::thicknessChanged()
{
    // update dashboard
    ui->thickness_recipe->setText(m_mainCTL.getRecipe()->getThicknessQStr());
}

// update recipe gap
void MainWindow::gapChanged()
{
    // update dashboard
    ui->gap_recipe->setText(m_mainCTL.getRecipe()->getGapQStr());
}

// update recipe overlap
void MainWindow::overlapChanged()
{
    // update dashboard
    ui->overlap_recipe->setText(m_mainCTL.getRecipe()->getOverlapQStr());
}

// update recipe speed
void MainWindow::speedChanged()
{
    // update dashboard
    ui->speed_recipe->setText(m_mainCTL.getRecipe()->getSpeedQStr());
}

// update recipe auto scan
void MainWindow::autoScanChanged()
{
    (m_mainCTL.getRecipe()->getAutoScanBool()) ? ui->autoscan_recipe->setText("ON") : ui->autoscan_recipe->setText("OFF");
}

// update recipe xlimits
void MainWindow::xLimitsChanged()
{
    // update dashboard
    ui->x1_recipe->setText(m_mainCTL.getRecipe()->getXminPHQStr());
    ui->x2_recipe->setText(m_mainCTL.getRecipe()->getXmaxPHQStr());
}

// update recipe y limits
void MainWindow::yLimitsChanged()
{
    // update dashboard
    ui->y1_recipe->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->y2_recipe->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());
}

// update recipe cycles
void MainWindow::cyclesChanged()
{
    // update dashboard
    ui->cycles_recipe->setText(m_mainCTL.getRecipe()->getCyclesQStr());
}

void MainWindow::AutoTuneCheckbox_stateChanged(int value)
{
    m_mainCTL.getTuner().setAutoTune(value);
}

//////////////////////////////////////////////////////////////////////////////////
// runtime updates
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::forwardWattsChanged()
{
    int watts = m_mainCTL.getPower().getForwardWatts();

    // dashboard
    ui->RF_Actual_LCD->display(watts);
    ui->RF_bar->setMaximum(m_mainCTL.getPower().getMaxForwardWatts());
    ui->RF_bar->setValue(watts);
}

void MainWindow::reflectedWattsChanged()
{
    int watts = m_mainCTL.getPower().getReflectedWatts();

    // dashboard
    ui->RefRF_Actual_LCD->display(watts);
    ui->RefRF_bar->setValue(watts);
}

void MainWindow::MBactualPositionChanged(const double actualPosition)
{
    //double testPosition = 55.00;
    QString sDisplayValue = QString::number(actualPosition, 'f', 2);
    // dashboard
    ui->MB_Actual_LCD->display(sDisplayValue);
    ui->MB_Pos_Bar->setValue(int(actualPosition));
}

void MainWindow::headTemperatureChanged()
{
    double temp = m_mainCTL.getPlasmaHead().getTemperature();

    // dashboard
    ui->temp_LCD->display(temp);
    ui->Temp_bar->setValue(int(temp));
}

void MainWindow::actualFlowChanged(const int mfcNumber, const double actualFlow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(1)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // set max
       ui->gas1_max_label->setText(QString::number(range, 'f', 1));

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas1ProgressBar->setValue(int(percentage));
       } else
           ui->gas1ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas1_actual_SLPM->setText(QString::number(actualFlow));
    }
    else if (mfcNumber == 2) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();

       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // set max
       ui->gas2_max_label->setText(QString::number(range, 'f', 1));

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas2ProgressBar->setValue(int(percentage));
       } else
           ui->gas2ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas2_actual_SLPM->setText(QString::number(actualFlow));
    }
    else if (mfcNumber == 3) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // set max
       ui->gas3_max_label->setText(QString::number(range, 'f', 1));

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas3ProgressBar->setValue(int(percentage));
       } else
           ui->gas3ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas3_actual_SLPM->setText(QString::number(actualFlow));
    }
    else if (mfcNumber == 4) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // set max
       ui->gas4_max_label->setText(QString::number(range, 'f', 1));

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas4ProgressBar->setValue(int(percentage));
       } else
           ui->gas4ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas4_actual_SLPM->setText(QString::number(actualFlow));
    }
    else if (mfcNumber == 5) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(5)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // set max
       ui->gas5_max_label->setText(QString::number(range, 'f', 1));

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas5ProgressBar->setValue(int(percentage));
       } else
           ui->gas5ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas5_actual_SLPM->setText(QString::number(actualFlow));
    }
    else if (mfcNumber == 6) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(6)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

       // update the progress bar
       if (percentage > 5) {// progress bars don't look right when value is less than 5
           ui->gas6ProgressBar->setValue(int(percentage));
       } else
           ui->gas6ProgressBar->setValue(0);

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas6_actual_SLPM->setText(QString::number(actualFlow));
    }
}

void MainWindow::rangeChanged(const int mfcNumber, double range)
{
    if (mfcNumber == 1) {
        // set max
        ui->gas1_max_label->setText(QString::number(range, 'f', 1));
    }
    else if (mfcNumber == 2) {
        // set max
        ui->gas2_max_label->setText(QString::number(range, 'f', 1));
    }
    else if (mfcNumber == 3) {
        // set max
        ui->gas3_max_label->setText(QString::number(range, 'f', 1));
    }
    else if (mfcNumber == 4) {
        // set max
        ui->gas4_max_label->setText(QString::number(range, 'f', 1));
    }
    else if (mfcNumber == 5) {
        // set max
        ui->gas5_max_label->setText(QString::number(range, 'f', 1));
    }
    else if (mfcNumber == 6) {
        // set max
        ui->gas6_max_label->setText(QString::number(range, 'f', 1));
    }
}

void MainWindow::userEnteredPassword()
{
    QString enteredPassword = m_passDialog.getUserEnteredPassword();

    // clear the password
    m_passDialog.clearPassword();

    QString password = m_config.getValueForKey(CONFIG_PASSWORD_KEY);

    if (enteredPassword == password) {
       // good password, update the UI
       setUIEngineerMode();
       m_engineeringMode = true;
    }
    else {
        QMessageBox::critical(this, "Error", "Incorrect password");
    }
}

//  update plasma button if the controller disabled plasma for some reason
void MainWindow::plasmaStateChanged(bool plasmaActive)
{
    if (plasmaActive) {
        ui->plsmaBtn->setText("PLASMA OFF");
    }
    else {
        ui->plsmaBtn->setChecked(false);
        ui->plsmaBtn->setText("START PLASMA");
    }
}
//////////////////////////////////////////////////////////////////////////////////
// cascade recipe
//////////////////////////////////////////////////////////////////////////////////

// populate a QListWidget with recipe names from the recipe directory
void MainWindow::populateRecipeListWidgetFromDirectory(QListWidget* listWidget)
{
    listWidget->clear(); // Clear the list widget before populating it

    QDir directory(RECIPE_DIRECTORY);

    if (!directory.exists()) {
        // Handle the case where the directory doesn't exist
        QMessageBox::information(this, "Error", "Directory: " + QString(RECIPE_DIRECTORY) + " does not exist");
        qDebug() << "Directory does not exist: " << RECIPE_DIRECTORY;
        return;
    }

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    foreach (const QFileInfo& fileInfo, fileInfoList) {
        // Add each file name to the list widget
        QListWidgetItem* item = new QListWidgetItem(fileInfo.fileName());
        listWidget->addItem(item);
    }
}

//save cascade recipe button
void MainWindow::on_saveAsCascadeRecipeButton_clicked()
{
    // Open the file dialog for saving
    QString selectedFileName = QFileDialog::getSaveFileName(this, "Save Cascade Recipe List", CASCADE_RECIPE_DIRECTORY, "Cascade Recipe Files (*.crcp)");
    if (!selectedFileName.isEmpty()) {
       // Create the file path
       QString filePath = selectedFileName;

       // Open the file for writing
       QFile file(filePath);
       QFileInfo fileInfo(file);

       // get the filename without the extension
       QString fileName = fileInfo.completeBaseName();

       // determine if we need to add the file extension
       if (fileInfo.completeSuffix() != "crcp") {
           file.setFileName(filePath + ".crcp");
       }

       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            int itemCount = ui->listCascadeRecipes->count();

            for (int i = 0; i < itemCount; i++) {
                QListWidgetItem* item = ui->listCascadeRecipes->item(i);
                QString recipeName = item->text();
                out << recipeName << "\n";
            }

            // update the cascade Recipe Field
            ui->cascade_recipe_name->setText(fileName);

            file.close();
            qDebug() << "Cascade recipe list saved to file: " << filePath;
       } else {
            // Display a success message box
            QMessageBox::information(this, "Error", "Cascade recipe '" + fileName + "' was unable to be saved successfully!");
            qDebug() << "Failed to open file for writing: " << file.errorString();
       }
    }

}

// load cascade recipe button
void MainWindow::on_loadCascadeRecipeButton_clicked()
{
    // Create a file dialog
    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::ExistingFile);

    // Set the initial directory
    dialog.setDirectory(CASCADE_RECIPE_DIRECTORY);
    // Set the window title and filter for specific file types
    dialog.setWindowTitle("Open Cascade Recipe File");
    dialog.setNameFilter("Cascade Recipe Files (*.crcp)");

    // Execute the file dialog
    if (dialog.exec()) {
       // Get the selected file path
       QString filePath = dialog.selectedFiles().first();

       // Read the content of the file
       QFile file(filePath);
       QFileInfo fileInfo(file);

       // get the filename without the extension
       QString fileName = fileInfo.completeBaseName();

       if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);

            // Clear existing items in the list widget
            ui->listCascadeRecipes->clear();

            // clear existing items in the backend list
            m_mainCTL.getRecipe()->clearCascadeRecipes();

            bool firstRecipe = true;

            // Read each line from the file and add it to the list widget
            while (!in.atEnd()) {
                QString line = in.readLine();
                QListWidgetItem *item = new QListWidgetItem(line);
                ui->listCascadeRecipes->addItem(item);

                // add recipe to backend list
                m_mainCTL.getRecipe()->addRecipeToCascade(item->text());

                // the first recipe in the list get's installed into the system
                if (firstRecipe) {
                    installRecipe(QString(RECIPE_DIRECTORY) + "/" + line);
                    firstRecipe = false;
                    
                    // set the cascade recipe index to 1
                    m_mainCTL.getRecipe()->resetCascadeIndex();
                }
            }

            // update the cascade Recipe Field
            ui->cascade_recipe_name->setText(fileName);
            QApplication::processEvents();

            // Close the file
            file.close();

            Logger::logInfo("Cascade Recipe opened: " + filePath);
       } else {
            Logger::logCritical("Failed to open file for reading: " + file.errorString());
       }
    } else {
       // User canceled the file dialog
       Logger::logDebug("File selection canceled.");
    }
}

// add cascade recipe button
void MainWindow::on_addCascadeRecipeButton_clicked()
{
    // Get the selected item from the source list
    QListWidgetItem* selectedItem = ui->listRecipes->currentItem();

    if (selectedItem) {
       // Add the item to the destination list
       ui->listCascadeRecipes->addItem(selectedItem->text());
       ui->cascade_recipe_name->setText("*modified*");

       // add recipe to backend list
       m_mainCTL.getRecipe()->addRecipeToCascade(selectedItem->text());
    }
}

// remove cascade recipe button
void MainWindow::on_removeCascadeRecipeButton_clicked()
{
    QString test = ui->listCascadeRecipes->currentItem()->text();
    // remove single recipe from backend list
    m_mainCTL.getRecipe()->removeRecipeFromCascade(test);

    ui->cascade_recipe_name->setText("*modified*");
    qDeleteAll(ui->listCascadeRecipes->selectedItems());
}

// clear cascade recipe button
void MainWindow::on_clear_cascade_recipe_button_clicked()
{
    ui->listCascadeRecipes->clear();

    // clear the backend list
    m_mainCTL.getRecipe()->clearCascadeRecipes();
}

void MainWindow::on_refresh_cascade_recipe_button_clicked()
{
    // Setup Recipe List for Cascade Recipes
    populateRecipeListWidgetFromDirectory(ui->listRecipes);
}

// slot to load the next cascade recipe.
// signal sent from plasmacontroller
void MainWindow::loadCascadeRecipe()
{
    int currentCascadeIndex = m_mainCTL.getRecipe()->getCurentCascadeIndex();
    int numCascadeRecipes = m_mainCTL.getRecipe()->getNumCascadeRecipes();

    if (currentCascadeIndex < numCascadeRecipes) { // load next recipe in the list
       QString recipeName = m_mainCTL.getRecipe()->getCascadeRecipeList().at(currentCascadeIndex);

       installRecipe(QString(RECIPE_DIRECTORY) + "/" + recipeName);
    }
    else { // we are done so install the first recipe in case user wants to run again
       QString recipeName = m_mainCTL.getRecipe()->getCascadeRecipeList().at(0);

       installRecipe(QString(RECIPE_DIRECTORY) + "/" + recipeName);
       QApplication::processEvents();

       // and reset our index in case the user wants to run the list again
       m_mainCTL.getRecipe()->resetCascadeIndex();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Button handlers
//////////////////////////////////////////////////////////////////////////////////

// init button on dash
void MainWindow::on_init_button_clicked()
{
    m_mainCTL.getAxesController().StartInit();
}

// home button on dash
void MainWindow::on_Home_button_toggled(bool checked)
{
    if (checked) {
       m_mainCTL.getAxesController().StartHome();
    }
    else {
       ui->Home_button->setChecked(false);
       m_mainCTL.getAxesController().StopHome();
    }
}

// scan button on dashboard
void MainWindow::on_scan_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.StartScan();
    }
    else {
        ui->scan_button->setChecked(false);
        m_mainCTL.StopScan();
    }
}

// pins on dashboard
void MainWindow::on_Stagepins_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().togglePinsOn();
    }
    else {
        m_mainCTL.getAxesController().togglePinsOff();
    }
}

// n2 purge button on dashboard
void MainWindow::on_n2_purge_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleN2PurgeOn();
    }
    else {
        m_mainCTL.getAxesController().toggleN2PurgeOff();
    }

}

// joystick button on dashboard
void MainWindow::on_Joystick_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleJoystickOn();
    }
    else {
        m_mainCTL.getAxesController().toggleJoystickOff();ui->menuStage_Test->setEnabled(false);
    }
}

// diameter button on dashboard
void MainWindow::on_diameter_button_clicked()
{
    // get the index of the current displayed diameter
    int comboboxCurrentIndex = ui->wafer_diameter->currentIndex();

    // set our diameter object
    m_mainCTL.getDiameter().setCurrentWaferDiameter(m_mainCTL.getDiameter().getWaferDiameterByIndex(comboboxCurrentIndex));

    // update the combox box on the 3axis tab
    ui->wafer_diameter->setCurrentIndex(comboboxCurrentIndex);

    // reset the scan box
    m_mainCTL.runDiameter();
}


// wafer combo box on the dashboard
void MainWindow::on_wafer_diameter_currentIndexChanged(int index)
{
    // get the index of the current displayed diameter
    int comboboxCurrentIndex = index;

    // get the current diameter
    int currentDiameter = m_mainCTL.getDiameter().getCurrentWaferDiameterSelection();

    // proposed diameter
    int proposedDiameter = m_mainCTL.getDiameter().getWaferDiameterByIndex(comboboxCurrentIndex);

    if (currentDiameter != proposedDiameter) {

        // set our diameter object
        m_mainCTL.getDiameter().setCurrentWaferDiameter(m_mainCTL.getDiameter().getWaferDiameterByIndex(comboboxCurrentIndex));

        // update the combo box on the dashboard tab
        ui->wafer_diameter->setCurrentIndex(comboboxCurrentIndex);
    }
}

// vacuum button on dashboard
void MainWindow::on_vac_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleVacOn();
    }
    else {
        m_mainCTL.getAxesController().toggleVacOff();
    }
}

// plasma button on dashboard
void MainWindow::on_plsmaBtn_toggled(bool checked)
{
    if (!checked) {
        m_mainCTL.StopScan();
        m_mainCTL.RunRecipe(false); // turn off recipe execution
        ui->plsmaBtn->setText("START PLASMA");
    }
    else {
        ui->plsmaBtn->setText("PLASMA OFF");

        if (m_mainCTL.getCollision() && m_mainCTL.getRecipe()->getAutoScanBool() && !m_mainCTL.getPlasmaActive()) {
            m_mainCTL.plannedAutoStartOn();//this will make sure we dont accidently start plasma when just clicking RUN SCAN button
            m_mainCTL.StartScan();
        }
        else {
            m_mainCTL.RunRecipe(true); // turn on recipe execution
        }
    }
}

// thickness button on dashboard
void MainWindow::on_load_thick_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    //m_pRecipeInputDialog = new QInputDialog(this);
    m_pRecipeInputDialog->setWindowTitle("Substrate Thickness in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);


    QString msg = "Format xx.yy (max value: ";
    double maxVal = m_mainCTL.getAxesController().getZp2BaseDbl() - m_mainCTL.getRecipe()->getThickness();
    msg += QString::number(maxVal, 'f', 2) + ")";

    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_ThicknessAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_ThicknessAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();
    double maxVal = m_mainCTL.getAxesController().getZp2BaseDbl() - m_mainCTL.getRecipe()->getThickness();
    if (!recipeStr.isEmpty()) {
        bool validNum;
        double recipeVal = recipeStr.toDouble(&validNum);
        if (validNum && recipeVal <= maxVal && recipeVal >= 0) {
            m_mainCTL.getRecipe()->setThickness(recipeVal);
        }
    }
}

// gap button on dashboard
void MainWindow::on_load_gap_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Plasma Z Gap in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format xx.yy (min value: 0.5) (max value: ";
    double maxVal = m_mainCTL.getAxesController().getZp2BaseDbl() - m_mainCTL.getRecipe()->getThickness();
    msg += QString::number(maxVal, 'f', 2) + ")   ";

    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_GapAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_GapAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();
    double maxVal = m_mainCTL.getAxesController().getZp2BaseDbl() - m_mainCTL.getRecipe()->getThickness();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double recipeVal = recipeStr.toDouble(&validNum);
        if (validNum && recipeVal <= maxVal && recipeVal >= 0.5) {
            m_mainCTL.getRecipe()->setGap(recipeVal);
        }
    }
}

// overlap button on dashboard
void MainWindow::on_load_overlap_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Overlap in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format xx.yy (max value: " + m_mainCTL.getPlasmaHead().getSlitLengthQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_OverlapAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_OverlapAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double recipeVal = recipeStr.toDouble(&validNum);
        if (validNum && recipeVal <= m_mainCTL.getPlasmaHead().getSlitLength() && recipeVal >= 0) {
            m_mainCTL.getRecipe()->setOverlap(recipeVal);
        }
    }
}

// speed button on dashboard
void MainWindow::on_loadSpeedButton_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Speed Enter mm/sec Value");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format xx.yy (max value: " + m_mainCTL.getAxesController().getYAxisMaxSpeedQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_SpeedAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_SpeedAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double recipeVal = recipeStr.toDouble(&validNum);
        if (validNum && recipeVal <= m_mainCTL.getAxesController().YMaxSpeed() && recipeVal > 0) {
            m_mainCTL.getRecipe()->setSpeed(recipeVal);
        }
    }
}

// cycles button on dashboard
void MainWindow::on_load_cycles_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Cycles Enter Integer Value");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format xxx (max value: 100)       ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_CyclesAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_CyclesAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        int recipeVal = recipeStr.toInt(&validNum);
        if (validNum && recipeVal <= 100 && recipeVal > 0) {
            m_mainCTL.getRecipe()->setCycles(recipeVal);
        }
    }
}

// open recipe button
void MainWindow::on_loadRecipeButton_clicked()
{
    openRecipe();
}

// save recipe button
void MainWindow::on_save_recipe_button_clicked()
{
    saveRecipe();
}

void MainWindow::openRecipeWindowMFC()
{
    QString strRange;
    int mfcNumber;
    MFC* mfc = nullptr;

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // Retrieve the MFC number from the button's property
        mfcNumber = button->property("MFCNumber").toInt();  // Retrieve the MFC index from the button's property
        mfc = m_mainCTL.findMFCByNumber(mfcNumber);
        if (mfc) {
            strRange = mfc->getRangeQString();
        }
    }

    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");
    m_pRecipeInputDialog->setWindowTitle("MFC Setpoint");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);
    m_pRecipeInputDialog->setProperty("MFC_Num", mfcNumber);

    QString msg = "Format xx.yy (max value: " + strRange + ")   ";

    m_pRecipeInputDialog->setLabelText(msg);

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_MFCSetpointAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}


void MainWindow::Recipe_MFCSetpointAccepted()
{
    bool ok;
    QString recipeStr = m_pRecipeInputDialog->textValue();
    int mfcNumber = m_pRecipeInputDialog->property("MFC_Num").toInt(&ok);

    if (ok && !recipeStr.isEmpty()) { // User entered a string and clicked OK
        MFC *mfc = m_mainCTL.findMFCByNumber(mfcNumber);
        bool validNum;
        double enteredValue = recipeStr.toDouble(&validNum);
        double range = mfc->getRange();
        if (validNum && enteredValue <= range && enteredValue >= 0 && mfc) {
            mfc->setRecipeFlow(enteredValue);
        }
    }
}

void MainWindow::on_loadRFButton_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("RF Setpoint");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);


    QString msg = "Format xxx (max value: ";
    msg += m_mainCTL.getPower().getMaxForwardWattsQStr() + ")   ";

    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_RFSetpointAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_RFSetpointAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();
    
    if (!recipeStr.isEmpty()) { // User entered a string and clicked OK
        bool validNum;
        int recipe = recipeStr.toInt(&validNum);
        if (validNum && recipe >= 0 && recipe <= m_mainCTL.getPower().getMaxForwardWatts()) {
            m_mainCTL.getPower().setRecipeWatts(recipe);
        }
    }
}

void MainWindow::on_loadMBButton_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Tuner Position Enter Percentage");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format xxx.xx (max value: 100)   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_MBSetpointAccepted);
    
    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_MBSetpointAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();
    
    if (!recipeStr.isEmpty()) { 
        bool validNum;
        double recipeVal = recipeStr.toDouble(&validNum);
        if (validNum && recipeVal >= 0.0 && recipeVal <= 100.0) {
            m_mainCTL.getTuner().setRecipePosition(recipeVal);
        }
    }
}


void MainWindow::on_loadAutoTuneButton_clicked()
{
    if (ui->autotune_recipe->text() == "OFF") {
        m_mainCTL.getTuner().setAutoTune(true);
        ui->autotune_recipe->setText("ON");
    }
    else {
        m_mainCTL.getTuner().setAutoTune(false);
        ui->autotune_recipe->setText("OFF");
    }
}

void MainWindow::on_load_autoscan_clicked()
{
    if (ui->autoscan_recipe->text() == "OFF") {
        m_mainCTL.getRecipe()->setAutoScan(true);
        ui->autoscan_recipe->setText("ON");
    }
    else {
        m_mainCTL.getRecipe()->setAutoScan(false);
        ui->autoscan_recipe->setText("OFF");
    }
}

void MainWindow::on_x1_set_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Box X min in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format +/- xxx.yy (max value: ";
    msg += m_mainCTL.getAxesController().getXPH2BaseQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_X1Accepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_X1Accepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double x1 = recipeStr.toDouble(&validNum);
        if (validNum && x1 <= m_mainCTL.getAxesController().getXPH2Base() && x1 >= (-1*m_mainCTL.getAxesController().getXPH2Base())) {
            m_mainCTL.getRecipe()->setXminPH(x1);
        }
    }
}

void MainWindow::on_x2_set_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Box X min in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format +/- xxx.yy (max value: ";
    msg += m_mainCTL.getAxesController().getXPH2BaseQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_X2Accepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_X2Accepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double x2 = recipeStr.toDouble(&validNum);
        if (validNum && x2 <= m_mainCTL.getAxesController().getXPH2Base() && x2 >= (-1*m_mainCTL.getAxesController().getXPH2Base())) {
            m_mainCTL.getRecipe()->setXmaxPH(x2);
        }
    }
}

void MainWindow::on_y1_set_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Box X min in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format +/- xxx.yy (max value: ";
    msg += m_mainCTL.getAxesController().getYPH2BaseQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_Y1Accepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_Y1Accepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double y1 = recipeStr.toDouble(&validNum);
        if (validNum && y1 <= m_mainCTL.getAxesController().getYPH2Base() && y1 >= (-1*m_mainCTL.getAxesController().getYPH2Base())) {
            m_mainCTL.getRecipe()->setYminPH(y1);
        }
    }
}

void MainWindow::on_y2_set_clicked()
{
    // clear the previous value
    m_pRecipeInputDialog->setTextValue("");

    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Scan Box X min in mm");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Format +/- xxx.yy (max value: ";
    msg += m_mainCTL.getAxesController().getYPH2BaseQStr() + ")   ";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_Y2Accepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_Y2Accepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        bool validNum;
        double y2 = recipeStr.toDouble(&validNum);
        if (validNum && y2 <= m_mainCTL.getAxesController().getYPH2Base() && y2 >= (-1*m_mainCTL.getAxesController().getYPH2Base())) {
            m_mainCTL.getRecipe()->setYmaxPH(y2);
        }
    }
}

// right arrow button
void MainWindow::on_MB_Right_Button_clicked()
{
    m_mainCTL.MBRight();
}

// left arrow button
void MainWindow::on_MB_Left_Button_clicked()
{
    m_mainCTL.MBLeft();
}

// Engineer choice from service menu
void MainWindow::on_actionEngineer_Mode_triggered()
{
    m_passDialog.show();
}

// Operator choice from service menu
void MainWindow::on_actionOperator_Mode_triggered()
{
    m_engineeringMode = false;
    setUIOperatorMode();
}

void MainWindow::disableControlButtons()
{
    ui->Joystick_button->setEnabled(false);
    ui->vac_button->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->Stagepins_button->setEnabled(false);
    ui->menuStage_Test->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->wafer_diameter->setEnabled(false);
    ui->n2_purge_button->setEnabled(false);
    ui->plsmaBtn->setEnabled(false);
    ui->MB_Right_Button->setEnabled(false);
    ui->MB_Left_Button->setEnabled(false);
    ui->menuStage_Test->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->batchIDButton->setEnabled(false);
    ui->batchIDedit->setEnabled(false);
}

void MainWindow::setUIEngineerMode()
{
    QString sTitle = this->windowTitle();
    sTitle += "      ENGINEER MODE";
    this->setWindowTitle(sTitle);

    // enable recipe setpoint buttons
    ui->loadMFC1Button->setEnabled(true);
    ui->loadMFC2Button->setEnabled(true);
    ui->loadMFC3Button->setEnabled(true);
    ui->loadMFC4Button->setEnabled(true);
    ui->loadMFC5Button->setEnabled(true);
    ui->loadMFC6Button->setEnabled(true);
    ui->load_thick->setEnabled(true);
    ui->load_gap->setEnabled(true);
    ui->load_autoscan->setEnabled(true);
    ui->load_cycles->setEnabled(true);
    ui->load_overlap->setEnabled(true);
    ui->x1_set->setEnabled(true);
    ui->x2_set->setEnabled(true);
    ui->y1_set->setEnabled(true);
    ui->y2_set->setEnabled(true);
    ui->loadRFButton->setEnabled(true);
    ui->loadMBButton->setEnabled(true);
    ui->loadAutoTuneButton->setEnabled(true);
    ui->loadSpeedButton->setEnabled(true);
    // only enable these if we have a connection
    if (m_mainCTL.isOpen()) {
        ui->n2_purge_button->setEnabled(true);
        ui->plsmaBtn->setEnabled(true);
        ui->MB_Right_Button->setEnabled(true);
        ui->MB_Left_Button->setEnabled(true);
    }
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(true);
    ui->batchIDButton->setEnabled(true);
    ui->batchIDedit->setEnabled(true);
    // tool settings tab
    ui->mainTabWidget->setTabEnabled(2, true);
    // if initialized enable
    if (m_mainCTL.getAxesController().getAxesInitilizedStatus()) {
        ui->Joystick_button->setEnabled(true);
        ui->vac_button->setEnabled(true);
        ui->scan_button->setEnabled(true);
        ui->Home_button->setEnabled(true);
        ui->Stagepins_button->setEnabled(true);
        ui->menuStage_Test->setEnabled(true);
        ui->diameter_button->setEnabled(true);
        ui->wafer_diameter->setEnabled(true);
    }
}
void MainWindow::setUIOperatorMode()
{
    // remove ENGINEERING MODE from title bar
    this->setWindowTitle("ONTOS3 INTERFACE v" + QString(APP_VERSION));

    // disable recipe setpoint buttons
    ui->loadMFC1Button->setEnabled(false);
    ui->loadMFC2Button->setEnabled(false);
    ui->loadMFC3Button->setEnabled(false);
    ui->loadMFC4Button->setEnabled(false);
    ui->loadMFC5Button->setEnabled(false);
    ui->loadMFC6Button->setEnabled(false);
    ui->load_thick->setEnabled(false);
    ui->load_gap->setEnabled(false);
    ui->load_autoscan->setEnabled(false);
    ui->load_cycles->setEnabled(false);
    ui->load_overlap->setEnabled(false);
    ui->x1_set->setEnabled(false);
    ui->x2_set->setEnabled(false);
    ui->y1_set->setEnabled(false);
    ui->y2_set->setEnabled(false);
    ui->loadRFButton->setEnabled(false);
    ui->loadMBButton->setEnabled(false);
    ui->loadAutoTuneButton->setEnabled(false);
    ui->loadSpeedButton->setEnabled(false);
    // disable control buttons
    ui->diameter_button->setEnabled(false);
    ui->n2_purge_button->setEnabled(false);
    ui->wafer_diameter->setEnabled(false);
    ui->plsmaBtn->setEnabled(false);
    ui->MB_Right_Button->setEnabled(false);
    ui->MB_Left_Button->setEnabled(false);
    ui->menuStage_Test->setEnabled(false);
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(false);
    ui->batchIDButton->setEnabled(true);
    ui->batchIDedit->setEnabled(true);
    // tool settings tab
    ui->mainTabWidget->setTabEnabled(2, false);
    // put in not initilized UI state
    if (!m_mainCTL.getAxesController().getAxesInitilizedStatus()) {
        ui->Joystick_button->setEnabled(false);
        ui->vac_button->setEnabled(false);
        ui->scan_button->setEnabled(false);
        ui->Home_button->setEnabled(false);
        ui->Stagepins_button->setEnabled(false);
    }
}

// Set Default form service menu
void MainWindow::on_actionSet_Default_triggered()
{
    m_mainCTL.handleSetDefaultRecipeCommand();
}

// About choice from service menu
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Ontos 3 Interface"),
                       tr("The <b>Ontos3 Interface</b> is the latest"
                          "modern GUI for Plasma applications."));
}

// Connect choice from serial menu
void MainWindow::on_actionConnect_triggered()
{
    SettingsDialog::Settings p = m_pSettings->settings();

    showStatusMessage(tr("Connecting to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

    if (!m_mainCTL.open(*m_pSettings)) {

        QMessageBox::critical(this, "Error", m_mainCTL.getPortErrorString());

        showStatusMessage("Open error" + m_mainCTL.getPortErrorString());
    }
}

// Disconnect choice from serial menu
void MainWindow::on_actionDisconnect_triggered()
{
    closeMainPort();
}

// Settings choice from serial menu
void MainWindow::on_actionSettings_triggered()
{
    m_pSettings->show();
}

// start stage test
void MainWindow::on_actionStart_triggered()
{
    emit m_mainCTL.getAxesController().STSM_TransitionStartup();
}

// stop stage test
void MainWindow::on_actionStop_triggered()
{
    emit m_mainCTL.getAxesController().STSM_TransitionShutdown();
}

// detailed log enable/disable
void MainWindow::on_actionDetailed_Log_toggled(bool arg1)
{
    m_mainCTL.getAxesController().setDetailedLog(arg1);
}

// test z enable/disable
void MainWindow::on_actionTest_Z_toggled(bool arg1)
{
    m_mainCTL.getAxesController().setTestZ(arg1);
}

// slot from PlasmaController
void MainWindow::batchIDLoggingIsActive()
{
    ui->batchID_checkBox->setChecked(true);

    // save the setting
    m_persistentSettings.setValue(BATCHID_ENABLED_SETTING, true);

    batchIDEnabled();
}

// batch ID system enabled
void MainWindow::on_batchID_checkBox_clicked(bool checked)
{
    m_mainCTL.batchIDLoggingOn(checked);

    // save the setting
    m_persistentSettings.setValue(BATCHID_ENABLED_SETTING, checked);

    batchIDEnabled();
}

void MainWindow::batchIDEnabled()
{
    if (ui->batchID_checkBox->isChecked()) {
        // show button and text box
        ui->batchIDButton->setEnabled(true);
        ui->batchIDedit->setEnabled(true);
    }
    else {
        // hide button and text box
        ui->batchIDButton->setEnabled(false);
        ui->batchIDedit->setEnabled(false);
    }
}

// crash avoidance system enabled/disabled
void MainWindow::on_collision_system_checkbox_clicked(bool checked)
{
    m_mainCTL.hasCollision(checked);

    // save the setting
    m_persistentSettings.setValue(COLLISION_SYSTEM_ENABLED_SETTING, checked);
}

void MainWindow::on_batchIDButton_clicked()
{
    // disconnect any previous connections
    disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

    m_pRecipeInputDialog->setWindowTitle("Batch ID #");
    m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);

    QString msg = "Enter the Batch ID #";
    m_pRecipeInputDialog->setLabelText(msg);
    connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &MainWindow::Recipe_BatchIDAccepted);

    m_pRecipeInputDialog->setFocus();
    m_pRecipeInputDialog->show();
    m_pRecipeInputDialog->adjustSize();
    QApplication::processEvents();
}

void MainWindow::Recipe_BatchIDAccepted()
{
    QString recipeStr = m_pRecipeInputDialog->textValue();

    if (!recipeStr.isEmpty()) {
        if (recipeStr == "" || recipeStr.length() > 45) {
            return;
        }
        else {
            Logger::logInfo("----------------------------BATCH ID # ---------------------------------");
            Logger::logInfo("Logging Batch ID #: " + recipeStr);
            ui->batchIDedit->setText(recipeStr);
        }
    }
}


// configure the gasses layout according to the number of
// MFC's returned by the main controller
void MainWindow::setUINumberOfMFCs(const int numMFCs)
{
    if (numMFCs == 5) { // if we have 5 MFC's hide MFC 6
        ui->gas6_widget->hide();
        ui->loadMFC6Button->hide();
        ui->mfc6_recipe->hide();
    }
    else if (numMFCs == 4) { // if we have 4 MFC's hide MFC 5 and 6
        ui->gas5_widget->hide();
        ui->gas6_widget->hide();
        ui->loadMFC6Button->hide();
        ui->loadMFC5Button->hide();
        ui->mfc5_recipe->hide();
        ui->mfc6_recipe->hide();
    }
}


void MainWindow::on_LEDIntensitySpinBox_valueChanged(double arg1)
{
    int intensity = arg1;
    m_mainCTL.setLEDLightIntensity(intensity);
}


