#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
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
    m_commandFileReader(),
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
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMStartup, this, &MainWindow::twoSpotStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMDone, this, &MainWindow::twoSpotStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::pinsStateChanged, this, &MainWindow::pinsStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::joystickStateChanged, this, &MainWindow::joystickStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::n2StateChanged, this, &MainWindow::n2StateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::vacStateChanged, this, &MainWindow::vacStateChanged);
    // ui updates from plasma controller and sub objects
    connect(&m_mainCTL, &PlasmaController::SSM_StatusUpdate, this, &MainWindow::SSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::SSM_Started, this, &MainWindow::SSM_Started);
    connect(&m_mainCTL, &PlasmaController::SSM_Done, this, &MainWindow::SSM_Done);
    connect(&m_mainCTL, &PlasmaController::CSM_StatusUpdate, this, &MainWindow::CSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::scanBoxChanged, this, &MainWindow::scanBoxChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::recipePositionChanged, this, &MainWindow::setRecipeMBtuner);
    connect(&m_mainCTL.getPlasmaHead(), &PlasmaHead::headTemperatureChanged, this, &MainWindow::headTemperatureChanged);
    connect(&m_passDialog, &PasswordDialog::userEnteredPassword, this, &MainWindow::userEnteredPassword);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::serialClosed, this, &MainWindow::serialDisconnected);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::serialOpen, this, &MainWindow::serialConnected);
    connect(m_mainCTL.getSerialInterface(), &SerialInterface::readTimeoutError, this, &MainWindow::readTimeoutError);
    connect(&m_mainCTL.getAbortMessages(), &AbortCodeMessages::showAbortMessageBox, this, &MainWindow::showAbortMessageBox);
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
    connect(&m_mainCTL.getPower(), &PWR::forwardWattsChanged, this, &MainWindow::forwardWattsChanged);
    connect(&m_mainCTL.getPower(), &PWR::reflectedWattsChanged, this, &MainWindow::reflectedWattsChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::autoTuneChanged, this, &MainWindow::autoTuneChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::actualPositionChanged, this, &MainWindow::MBactualPositionChanged);

    // disable until implemented
    //ui->mainTabWidget->setTabEnabled(1, false);
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

    // read persistent settings and update UI
    readSettings();

    // give things a little time to settle before opening the serial port
    QTimer::singleShot(50, this, &MainWindow::openMainPort);
}

MainWindow::~MainWindow() {
    delete m_pSettings;
    delete ui;

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

void MainWindow::closeEvent(QCloseEvent *event) {

    if (m_mainCTL.isOpen()) {
        m_mainCTL.close();
    }
    Logger::logInfo("Shutting Down -------------------");
    Logger::clean();
    MainWindow::close();
}

// style sheets take a bit to load when starting up.
// the effect is that the style sheets don't take full effect
// until after the mainwindow is visible.  This creates a little
// delay to handle that
void MainWindow::showEvent(QShowEvent *)
{
    QTimer::singleShot(50, this, &MainWindow::setInitialUIState);
}

void MainWindow::setInitialUIState()
{
    setUIOperatorMode();
    // show/hide button and text box
    batchIDEnabled();
}

//////////////////////////////////////////////////////////////////////////////////
// Startup/ Setup
//////////////////////////////////////////////////////////////////////////////////

void MainWindow::readSettings()
{
    // read the persistent settings
    bool batchID = m_persistentSettings.value(BATCHID_ENABLED_SETTING, false).toBool();
    bool collisionSystem = m_persistentSettings.value(COLLISION_SYSTEM_ENABLED_SETTING, false).toBool();
    bool heaterEnabled = m_persistentSettings.value(HEATER_ENABLED_SETTING, false).toBool();

    // update the UI
    ui->batchID_checkBox->setChecked(batchID);
    ui->collision_system_checkbox->setChecked(collisionSystem);
    ui->heater_checkbox->setChecked(heaterEnabled);
}

void MainWindow::connectRecipeButtons()
{
    //MFC buttons
    connectMFCRecipeButton(ui->loadMFC1Button, 1);
    connectMFCRecipeButton(ui->loadMFC2Button, 2);
    connectMFCRecipeButton(ui->loadMFC3Button, 3);
    connectMFCRecipeButton(ui->loadMFC4Button, 4);
}

void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < m_mainCTL.getMFCs().size(); ++i) {
        connect(m_mainCTL.getMFCs()[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeFlow);
        connect(m_mainCTL.getMFCs()[i], &MFC::actualFlowChanged, this, &MainWindow::actualFlowChanged);
    }
}

void MainWindow::setMFCLabels()
{
    QString MFC1_label = m_config.getValueForKey(CONFIG_MFC1_LABEL_KEY);
    QString MFC2_label = m_config.getValueForKey(CONFIG_MFC2_LABEL_KEY);
    QString MFC3_label = m_config.getValueForKey(CONFIG_MFC3_LABEL_KEY);
    QString MFC4_label = m_config.getValueForKey(CONFIG_MFC4_LABEL_KEY);

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
}

void MainWindow::connectMFCRecipeButton(QPushButton* button, const int& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}

void MainWindow::RunStartup()
{
    m_mainCTL.CTLStartup();
    m_mainCTL.getAxesController().AxisStartup();
}


void MainWindow::loadConfigGUI(QStringList value)
{
    ui->gas1_label->setText(value[0]);
    ui->gas2_label->setText(value[1]);
    ui->gas3_label->setText(value[2]);
    ui->gas4_label->setText(value[3]);
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
    ui->twospot_button->setEnabled(false);
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
        ui->twospot_button->setEnabled(true);
        ui->diameter_button->setEnabled(true);
    }
}

// set ui elements accordingly
void MainWindow::initStateMachineStartup()
{
    // disaable the init buttons
    ui->init_button->setEnabled(false);

    // disable the other state movement buttons
    ui->twospot_button->setEnabled(false);
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
        ui->twospot_button->setEnabled(true);
        ui->diameter_button->setEnabled(true);
        ui->wafer_diameter->setEnabled(true);
        ui->diameter_button->setEnabled(true);
        ui->menuStage_Test->setEnabled(true);
        ui->Joystick_button->setEnabled(true);
    }
}

void MainWindow::twoSpotStateMachineStartup()
{
    ui->twospot_button->setText("STOP");

    // disable the other stage movement buttons
    ui->Home_button->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->diameter_button->setEnabled(false);
}

void MainWindow::twoSpotStateMachineDone()
{
    ui->twospot_button->setChecked(false);
    ui->twospot_button->setText("TWO SPOT");

    scanBoxChanged();

    if (m_engineeringMode) {
        // enable the buttons that we disabled
        ui->Home_button->setEnabled(true);
        ui->init_button->setEnabled(true);
        ui->scan_button->setEnabled(true);
        ui->diameter_button->setEnabled(true);
    }
}

void MainWindow::SSM_Started()
{
    ui->scan_button->setText("STOP");

    // disable other stage movement buttons
    ui->init_button->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->twospot_button->setEnabled(false);
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
    QMessageBox::critical(this, "Error: ", "Read timeout on command " + lastCommand + " Exit");
    QApplication::exit(0);
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
    m_mainCTL.getAxesController().resetAxes();
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
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
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
            RunStatusPolling();
            m_mainCTL.setLightTower();
            m_mainCTL.RunDoorOpenSM();
            m_mainCTL.getAxesController().RunInitAxesSM();
            m_mainCTL.getAxesController().RunTwoSpotSM();
            m_mainCTL.getAxesController().RunStageTestSM();
            m_mainCTL.getAxesController().RunHomeAxesSM();
            m_mainCTL.RunScanAxesSM();
            m_mainCTL.RunCollisionSM();
            m_mainCTL.PollForCollision();
        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainIdleState)) { // in Idle state
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainShutdownState)) { // in Shutdown state
        Logger::logInfo("Main State Machine Shut Down");

        emit MSM_TransitionIdle();
    }
}

void MainWindow::RunStatusPolling()
{
    m_mainCTL.getCTLStatusCommand();
    m_mainCTL.getAxesController().getAxisStatus();
    AxisStatusToUI();
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

void MainWindow::showAbortMessageBox(QString message)
{
    QMessageBox::critical(this, "ABORT CONDITION", message);
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

// Mike: this works fine but should be refactored at some point
// This should probably come from a signal emitted from AxesController
// instead of polling.  Look at AxesController::checkAndLogAxesStatusChange()
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

void MainWindow::AutoTuneCheckbox_stateChanged(int value)
{
    m_mainCTL.getTuner().setAutoTune(value);
}

void MainWindow::openRecipeWindowMFC()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "MFC Setpoint", "Please enter a setpoint for the MFC:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        if (!m_mainCTL.getMFCs().isEmpty()) {
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            if (button) {

                // Retrieve the MFC number from the button's property
                int mfcNumber = button->property("MFCNumber").toInt();  // Retrieve the MFC index from the button's property
                MFC* mfc = m_mainCTL.findMFCByNumber(mfcNumber);
                if (mfc) {
                    double recipe = recipeStr.toDouble();
                    mfc->setRecipeFlow(recipe);
                }
            }
        }
    } else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::openRecipe()
{
    // Create a file dialog
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);

    QString initialDirectory = RECIPE_DIRECTORY;

    // Set the initial directory
    dialog.setDirectory(initialDirectory);
    // Set the window title and filter for specific file types
    dialog.setWindowTitle("Open Recipe File");
    dialog.setNameFilter("Recipe Files (*.rcp)");

    // Execute the file dialog
    if (dialog.exec()) {
        // Get the selected file path
        QString filePath = dialog.selectedFiles().first();

        // set the filename
        QFileInfo fi(filePath);
        ui->name_recipe->setText(fi.baseName());

        // set recipe path and file
        m_mainCTL.setRecipe(filePath);

        Logger::logInfo("Recipe opened: " + filePath);

    } else {
        // User canceled the file dialog
        Logger::logDebug("File selection canceled.");
    }
}

void MainWindow::saveRecipe() {
    // Create the directory path
    QString directoryPath = RECIPE_DIRECTORY;

    // Create the directory if it doesn't exist
    QDir directory;
    if (!directory.exists(directoryPath)) {
       directory.mkpath(directoryPath);
    }

    // Open the file dialog for saving
    QString selectedFileName = QFileDialog::getSaveFileName(this, "Save Recipe", directoryPath, "Recipe Files (*.rcp)");
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

            file.close();

            Logger::logInfo("Recipe saved to file: " + filePath);
       } else {
            Logger::logInfo("Failed to open file for writing: " + file.errorString());
       }
    }
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


//////////////////////////////////////////////////////////////////////////////////
// runtime updates
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::forwardWattsChanged()
{
    int watts = m_mainCTL.getPower().getForwardWatts();

    // dashboard
    ui->RF_Actual_LCD->display(watts);
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
    // dashboard
    ui->MB_Actual_LCD->display(actualPosition);
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
       ui->gas1ProgressBar->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas1_actual_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 2) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas2ProgressBar->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas2_actual_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 3) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas3ProgressBar->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas3_actual_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 4) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas4ProgressBar->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas4_actual_SLPM->setText(QString::number(percentage));
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

//////////////////////////////////////////////////////////////////////////////////
// cascade recipe
//////////////////////////////////////////////////////////////////////////////////

//save cascade recipe button
void MainWindow::on_saveAsCascadeRecipeButton_clicked()
{
    // Create the directory path
    QString directoryPath = QCoreApplication::applicationDirPath() + "/Cascade Recipes";

    // Create the directory if it doesn't exist
    QDir directory;
    if (!directory.exists(directoryPath)) {
       directory.mkpath(directoryPath);
    }

    // Open the file dialog for saving
    QString selectedFileName = QFileDialog::getSaveFileName(this, "Save Cascade Recipe List", directoryPath, "Text Files (*.txt)");
    if (!selectedFileName.isEmpty()) {
       // Create the file path
       QString filePath = selectedFileName;

       // Open the file for writing
       QFile file(filePath);
       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            // Write each recipe name to the file
            for (const QString& recipeName : m_mainCTL.getRecipe()->getCascadeRecipeList()) {
                out << recipeName << "\n";
            }

            file.close();
            qDebug() << "Cascade recipe list saved to file: " << filePath;
       } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
       }
    }

}

// load cascade recipe button
void MainWindow::on_loadCascadeRecipeButton_clicked()
{

}

// add cascade recipe button
void MainWindow::on_addCascadeRecipeButton_clicked()
{
    // Create a file dialog
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);

    // Get the current directory
    QString currentDirectory = QCoreApplication::applicationDirPath();
    QString initialDirectory = currentDirectory + "/recipes/";

    // Set the initial directory
    dialog.setDirectory(initialDirectory);
    // Set the window title and filter for specific file types
    dialog.setWindowTitle("Add Recipe File to Cascade Recipe");
    dialog.setNameFilter("Recipe Files (*.rcp)");

    // Execute the file dialog
    if (dialog.exec()) {
       // Get the selected file path
       QString filePath = dialog.selectedFiles().first();

       // Extract the file name from the file path
       QFileInfo fileInfo(filePath);
       QString fileName = fileInfo.fileName();

       // Set plasma Recipe path to Cascade Recipe
       m_mainCTL.getRecipe()->addRecipeToCascade(fileName);

       // Update the UI with the recipes
       ui->listCascadeRecipes->addItem(fileName);
    } else {
       // User canceled the file dialog
       qDebug() << "File selection canceled.";
    }

}

// remove cascade recipe button
void MainWindow::on_removeCascadeRecipeButton_clicked()
{
    // Get the selected item in the list widget
    QListWidgetItem* selectedItem = ui->listCascadeRecipes->currentItem();
    if (selectedItem) {
       // Get the text of the selected item
       QString recipeFileName = selectedItem->text();

       // Remove the item from the list widget
       ui->listCascadeRecipes->takeItem(ui->listCascadeRecipes->row(selectedItem));

       // Remove the item from the cascade recipe list
       m_mainCTL.getRecipe()->removeRecipeFromCascade(recipeFileName);
    }

}

// clear cascade recipe button
void MainWindow::on_clear_cascade_recipe_button_clicked()
{

}

// open cascade recipe button
void MainWindow::openCascadeRecipe()
{
    //  plasmaRecipe.currentRecipeIndex_;
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

// two spot on dashboard
void MainWindow::on_twospot_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().StartTwoSpot();
    }
    else {
        ui->twospot_button->setChecked(false);
        m_mainCTL.getAxesController().StopTwoSpot();
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
        ui->plsmaBtn->setText("START PLASMA");
    }
    else {
        ui->plsmaBtn->setText("PLASMA OFF");
        // hard coded until the checkbox is in place
        m_mainCTL.hasCollision(true);
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
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Thickness: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.getRecipe()->setThickness(doubVal);
    }
}

// gap button on dashboard
void MainWindow::on_load_gap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Gap: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.getRecipe()->setGap(doubVal);
    }
}

// overlap button on dashboard
void MainWindow::on_load_overlap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Overlap: ","mm: ", 0, 0, 5.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.getRecipe()->setOverlap(doubVal);
    }
}

// speed button on dashboard
void MainWindow::on_loadSpeedButton_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Speed: ","mm: " + m_mainCTL.getAxesController().getXAxisMaxSpeedQStr(), 0, 0,
                                             m_mainCTL.getAxesController().XMaxSpeed(), 0, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.getRecipe()->setSpeed(doubVal);
    }
}


// cycles button on dashboard
void MainWindow::on_load_cycles_clicked()
{
    bool ok;
    int intVal = QInputDialog::getInt(this, "Number of Cycles","cycles: ", 0, 0, 200, 0, &ok);
    if (ok) {
        m_mainCTL.getRecipe()->setCycles(intVal);
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

void MainWindow::on_loadRFButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "RF Setpoint", "Please enter an integer setpoint for RF Power:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        int recipe = recipeStr.toInt();
        m_mainCTL.getPower().setRecipeWatts(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::on_loadMBButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "Tuner Setpoint", "Please enter a percentage for MB Tuner:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        int recipe = recipeStr.toInt();
        m_mainCTL.getTuner().setRecipePosition(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::on_loadAutoTuneButton_clicked()
{
    QStringList items;
    items << "OFF" << "ON";

    bool ok;
    QString item = QInputDialog::getItem(nullptr, "Tuner Autotune", "Please choose an auto tune state", items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        if (item == "OFF") {
            m_mainCTL.getTuner().setAutoTune(false);
        }
        else {
            m_mainCTL.getTuner().setAutoTune(true);
        }
    }
}


void MainWindow::on_load_autoscan_clicked()
{
    QStringList items;
    items << "OFF" << "ON";

    bool ok;
    QString item = QInputDialog::getItem(nullptr, "Auto scan", "Please choose an auto scan state", items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        if (item == "OFF") {
            m_mainCTL.getRecipe()->setAutoScan(false);
        }
        else {
            m_mainCTL.getRecipe()->setAutoScan(true);
        }
    }
}

void MainWindow::on_x1_set_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "X min", "Please enter X min", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double x1 = input.toDouble();
        m_mainCTL.getRecipe()->setXminPH(x1);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::on_x2_set_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "X max", "Please enter X max", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double x2 = input.toDouble();
        m_mainCTL.getRecipe()->setXmaxPH(x2);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::on_y1_set_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Y min", "Please enter Y min", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double y1 = input.toDouble();
        m_mainCTL.getRecipe()->setYminPH(y1);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::on_y2_set_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Y max", "Please enter Y max", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double y2 = input.toDouble();
        m_mainCTL.getRecipe()->setYmaxPH(y2);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
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
    ui->twospot_button->setEnabled(false);
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
        ui->twospot_button->setEnabled(true);
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
    ui->twospot_button->setEnabled(false);
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

// heater on tool settings tab
void MainWindow::on_heater_checkbox_clicked(bool checked)
{
    m_mainCTL.heaterOn(checked);

    // save the setting
    m_persistentSettings.setValue(HEATER_ENABLED_SETTING, checked);
}


void MainWindow::on_batchIDButton_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Batch ID #", "Enter the Batch ID #", QLineEdit::Normal, "", &ok);

    if (ok) {
        QString input = ui->batchIDedit->toPlainText();
        if (input == "" || input.length() > 45) {
            return;
        }
        else {
            Logger::logInfo("----------------------------BATCH ID # ---------------------------------");
            Logger::logInfo("Logging Batch ID #: " + input);
            ui->batchIDedit->setText(input);
        }
    }
}

