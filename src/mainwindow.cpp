#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

int SM_PollCounter = 0;
const int SM_POLL_PERIOD = 5;
std::chrono::milliseconds CTLResetTimeOut = 0ms;


MainWindow::MainWindow(MainLoop* loop, QWidget *parent) :
    QMainWindow(parent),
    m_pMainLoop(loop),
    ui(new Ui::MainWindow),
    m_pStatus(new QLabel),
    m_passDialog(),
    m_pSettings(new SettingsDialog),
    m_mainCTL(),
    m_commandFileReader(),
    m_pMainCTLConsole(),
    //m_pStageCTLConsole(),
    //m_pStageWidget(new StageWidget(this)),
    m_config()
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE v" + QString(APP_VERSION));

    // setup the state machine
    setupMainStateMachine();

    // ui updates from axescontroller
    connect(&m_mainCTL.getAxesController(), &AxesController::stageStatusUpdate, this, &MainWindow::stageStatusUpdate);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMStartup, this, &MainWindow::homeStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMDone, this, &MainWindow::homeStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIInitSMStartup, this, &MainWindow::initStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIInitSMDone, this, &MainWindow::initStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMStartup, this, &MainWindow::twoSpotStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMDone, this, &MainWindow::twoSpotStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::pinsStateChanged, this, &MainWindow::pinsStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::joystickStateChanged, this, &MainWindow::joystickStateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::n2StateChanged, this, &MainWindow::n2StateChanged);
    connect(&m_mainCTL.getAxesController(), &AxesController::vacStateChanged, this, &MainWindow::vacStateChanged);
    // ui updates from plasma controller and sub objects
    connect(&m_mainCTL, &PlasmaController::recipeExecutionStateChanged, this, &MainWindow::recipeExecutionStateChanged);
    connect(&m_mainCTL, &PlasmaController::SSM_StatusUpdate, this, &MainWindow::SSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::SSM_Started, this, &MainWindow::SSM_Started);
    connect(&m_mainCTL, &PlasmaController::SSM_Done, this, &MainWindow::SSM_Done);
    connect(&m_mainCTL, &PlasmaController::CSM_StatusUpdate, this, &MainWindow::CSM_StatusUpdate);
    connect(&m_mainCTL, &PlasmaController::scanBoxChanged, this, &MainWindow::scanBoxChanged);
    connect(&m_mainCTL.getTuner(), &Tuner::recipePositionChanged, this, &MainWindow::setRecipeMBtuner);
    //connect(&m_mainCTL, &PlasmaController::plasmaHeadTemp, this, &MainWindow::plasmaHeadTemp);
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



    // Make signal/slot connections here
    connectRecipeButtons(); // TODO: remove these and replace with designer click handlers

    // recipe slots
    connectMFCFlowBars();


    // status bar
    ui->statusBar->addWidget(m_pStatus);

    // setup wafer diamter combo box
    ui->wafer_diameter->addItems(m_mainCTL.getDiameter().getWaferDiameterTextList());
    ui->wafer_diameter_dup->addItems(m_mainCTL.getDiameter().getWaferDiameterTextList());

    // give things a little time to settle before opening the serial port
    QTimer::singleShot(50, this, SLOT(openMainPort()));
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

void MainWindow::showStatusMessage(const QString &message)
{
    m_pStatus->setText(message);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Ontos 3 Interface"),
                       tr("The <b>Ontos3 Interface</b> is the latest"
                          "modern GUI for Plasma applications."));
}

void MainWindow::shutDownProgram() {

    if (m_mainCTL.isOpen()) {
        m_mainCTL.close();
    }
    Logger::clean();
    MainWindow::close();
}

//////////////////////////////////////////////////////////////////////////////////
// Startup/ Setup
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::consoleMainCTLSetup()
{
    // Step 1: Create an instance of the console class
    m_pMainCTLConsole = new Console(ui->mainTabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->mainTabWidget->addTab(m_pMainCTLConsole, "Main CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    //ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(m_pMainCTLConsole, &Console::getData, this, &MainWindow::writeMainPort);
}


void MainWindow::connectRecipeButtons()
{
    //connect(ui->loadMBButton, &QPushButton::clicked, this, &MainWindow::loadMBRecipeButton_clicked);
    //connect(ui->AutoTuneCheckBox, &QCheckBox::stateChanged, this, &MainWindow::AutoTuneCheckbox_stateChanged); // MCD: should this be a checkbox?

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
    for (int i = 0; i < m_mainCTL.m_mfcs.size(); ++i) {
        connect(m_mainCTL.m_mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeFlow);
        connect(m_mainCTL.m_mfcs[i], &MFC::actualFlowChanged, this, &MainWindow::actualFlowChanged);
    }
}

void MainWindow::connectMFCRecipeButton(QPushButton* button, const int& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}

void MainWindow::RunStartup()
{
    //GetExeCfg();
    //loadConfigGUI(Values); // TODO
    m_mainCTL.CTLStartup();
    m_mainCTL.getAxesController().AxisStartup();
}


void MainWindow::loadConfigGUI(QStringList value)
{
    //config.setMFC1(value[1]);
    ui->gas1_label->setText(value[0]);
    //config.setMFC2(value[3]);
    ui->gas2_label->setText(value[1]);
    //config.setMFC3(value[5]);
    ui->gas3_label->setText(value[2]);
    //config.setMFC4(value[7]);
    ui->gas4_label->setText(value[3]);
}


//////////////////////////////////////////////////////////////////////////////////
// State machine slots
//////////////////////////////////////////////////////////////////////////////////

// set ui elements accordingly
void MainWindow::homeStateMachineStartup()
{
    // put the button in the checked state and change text
    ui->Home_button->setText("STOP");
    ui->Home_button_dup->setText("STOP");
    ui->Home_button->setChecked(true);
    ui->Home_button_dup->setChecked(true);

    // disable other stage movement buttons
    ui->init_button->setEnabled(false);
    ui->init_button_dup->setEnabled(false);
    ui->twospot_button->setEnabled(false);
    ui->twospot_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->scan_button_dup->setEnabled(false);
}

// set ui elements accordingly
void MainWindow::homeStateMachineDone()
{
    // put the button in the unchecked state and change text
    ui->Home_button->setText("LOAD");
    ui->Home_button_dup->setText("LOAD");
    ui->Home_button->setChecked(false);
    ui->Home_button_dup->setChecked(false);

    // enable other stage movement buttons
    ui->init_button->setEnabled(true);
    ui->init_button_dup->setEnabled(true);
    ui->twospot_button->setEnabled(true);
    ui->twospot_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
    ui->scan_button->setEnabled(true);
    ui->scan_button_dup->setEnabled(true);
}

// set ui elements accordingly
void MainWindow::initStateMachineStartup()
{
    // put the button in the checked state
    ui->init_button->setChecked(true);
    ui->init_button_dup->setChecked(true);

    // disable the other state movement buttons
    ui->twospot_button->setEnabled(false);
    ui->twospot_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->scan_button_dup->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->Home_button_dup->setEnabled(false);
}

// set ui elements accordingly
void MainWindow::initStateMachineDone()
{
    // put the init buttons in the unchecked state
    ui->init_button->setChecked(false);
    ui->init_button_dup->setChecked(false);

    // enable the other stage movement buttons
    ui->twospot_button->setEnabled(true);
    ui->twospot_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
    ui->Home_button->setEnabled(true);
    ui->Home_button_dup->setEnabled(true);
    ui->scan_button->setEnabled(true);
    ui->scan_button_dup->setEnabled(true);
}

void MainWindow::twoSpotStateMachineStartup()
{
    // put the button in the checked state and change text
    ui->twospot_button->setText("STOP");
    ui->twospot_button_dup->setText("STOP");
    ui->twospot_button->setChecked(true);
    ui->twospot_button_dup->setChecked(true);

    // disable the other stage movement buttons
    ui->Home_button->setEnabled(false);
    ui->Home_button_dup->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->init_button_dup->setEnabled(false);
    ui->scan_button->setEnabled(false);
    ui->scan_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
}

void MainWindow::twoSpotStateMachineDone()
{
    // put the button in the unchecked state and change text
    ui->twospot_button->setText("TWO SPOT");
    ui->twospot_button_dup->setText("TWO SPOT");
    ui->twospot_button->setChecked(false);
    ui->twospot_button_dup->setChecked(false);

    scanBoxChanged();

    // enable the buttons that we disabled
    ui->Home_button->setEnabled(true);
    ui->Home_button_dup->setEnabled(true);
    ui->init_button->setEnabled(true);
    ui->init_button_dup->setEnabled(true);
    ui->scan_button->setEnabled(true);
    ui->scan_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
}

void MainWindow::SSM_Started()
{
    // put the button in the checked state and change text
    ui->scan_button->setText("STOP");
    ui->scan_button_dup->setText("STOP");
    ui->scan_button->setChecked(true);
    ui->scan_button_dup->setChecked(true);

    // disable other stage movement buttons
    ui->init_button->setEnabled(false);
    ui->init_button_dup->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->Home_button_dup->setEnabled(false);
    ui->twospot_button->setEnabled(false);
    ui->twospot_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
}

void MainWindow::SSM_Done()
{
    // put buttons in unchecked state and change text
    ui->scan_button->setChecked(false);
    ui->scan_button_dup->setChecked(false);
    ui->scan_button->setText("SCAN");
    ui->scan_button_dup->setText("SCAN");

    // update status
    QString status = "Scanning Completed";
    ui->axisstatus->setText(status);
    ui->axisstatus_2->setText("");
    ui->axisstatus_dup->setText(status);
    ui->axisstatus_2_dup->setText("");

    // enable other stage movement buttons
    ui->init_button->setEnabled(true);
    ui->init_button_dup->setEnabled(true);
    ui->Home_button->setEnabled(true);
    ui->Home_button_dup->setEnabled(true);
    ui->twospot_button->setEnabled(true);
    ui->twospot_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
}

void MainWindow::scanBoxChanged()
{   
    // 3 axis tab
    ui->xmin_controls_dup->setText(m_mainCTL.getRecipe()->getXminPHQStr());
    ui->xmax_controls_dup->setText(m_mainCTL.getRecipe()->getXmaxPHQStr());
    ui->ymin_controls_dup->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->ymax_controls_dup->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());

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
        // stop the main state machine
        emit MSM_TransitionShutdown();
        m_mainCTL.close();
    }

    // Disable Console
    m_pMainCTLConsole->setEnabled(false);

    // Update Status bar
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::openMainPort()
{
    const SettingsDialog::Settings p = m_pSettings->settings();

    if (m_mainCTL.open(*m_pSettings)) {

        // Terminal Tab setup for console commands
//        consoleMainCTLSetup();
//        m_pMainCTLConsole->setEnabled(true);
//        m_pMainCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        m_mainCTL.getAxesController().resetAxes();
        m_mainCTL.resetCTL();
        CTLResetTimeOut = 2500ms / m_pMainLoop->getTimerInterval();
        // (DEBUG_MODE) ? MainStateMachine.setState(IDLE) : MainStateMachine.setState(STARTUP); // TODO: Needs implementing

        // Give status on connect
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

        // start the main state machine
        emit MSM_TransitionStartup();

    } else {
        QMessageBox::critical(this, tr("Error"), m_mainCTL.getPortErrorString());

        showStatusMessage(tr("Open error"));
    }
}


void MainWindow::writeMainPort(const QByteArray &data)
{
    if (!m_mainCTL.sendCommand(data))
        qDebug() << "Command not sent to main CTL";
}

QString MainWindow::readMainPort()
{
    return m_mainCTL.readResponse();
}


void MainWindow::handleMainSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_mainCTL.getPortErrorString());
        Logger::logCritical(m_mainCTL.getPortErrorString());
        closeMainPort();
    }
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
    m_pMainStartupState->addTransition(this, SIGNAL(MSM_TransitionPolling()), m_pMainPollingState);
    m_pMainIdleState->addTransition(this, SIGNAL(MSM_TransitionStartup()), m_pMainStartupState);

    // shutdown transitions
    m_pMainStartupState->addTransition(this, SIGNAL(MSM_TransitionShutdown()), m_pMainShutdownState);
    m_pMainPollingState->addTransition(this, SIGNAL(MSM_TransitionShutdown()), m_pMainShutdownState);
    m_pMainIdleState->addTransition(this, SIGNAL(MSM_TransitionShutdown()), m_pMainShutdownState);

    // idle transitions
    m_pMainStartupState->addTransition(this, SIGNAL(MSM_TransitionIdle()), m_pMainIdleState);
    m_pMainPollingState->addTransition(this, SIGNAL(MSM_TransitionIdle()), m_pMainIdleState);
    m_pMainShutdownState->addTransition(this, SIGNAL(MSM_TransitionIdle()), m_pMainIdleState);

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



void MainWindow::UpdateStatus()
{
    /*// TODO: need to implement
     * didStatusBitsChange();
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
    }*/

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
            UpdateStatus();
        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainPollingState)) { // in Polling state
        // RunCheckInput(); // TODO: Need to implement

        SM_PollCounter += 1;
        if (SM_PollCounter >= SM_POLL_PERIOD) {
            SM_PollCounter = 0;
            RunPolling();
            UpdateStatus();
            // setLightTower(); TODO: Need to implement
            // HandleDoorAbort() TODO: Need to implement // run the Door Abort state machine
            m_mainCTL.getAxesController().RunInitAxesSM();
            m_mainCTL.getAxesController().RunTwoSpotSM();
            m_mainCTL.getAxesController().RunHomeAxesSM();
            m_mainCTL.RunScanAxesSM();
            m_mainCTL.RunCollisionSM();
            m_mainCTL.PollForCollision();
        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainIdleState)) { // in Idle state
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainShutdownState)) { // in Shutdown state
    }
}


void MainWindow::RunPolling()
{
    /*
    getCTLStatus(); // TODO: need to implement
    //didCTLStatusChange(); //is this for logging?
    splitRCV(); // TODO: need to implement
    //! [0]
    setStatusBitsFromPoll();// TODO: need to implement*/
    //UpdateStatus();// TODO: need to implement
    //! [1]
    /*setTunerPosition();// TODO: need to implement
    displayTunerPosition();// TODO: need to implement
    //! [2]
    setRFPower();// TODO: need to implement
    displayRFValue();// TODO: need to implement
    //! [3]
    setReflectedPower();// TODO: need to implement
    displayReflectedPower();// TODO: need to implement
    //! [4]
    //! setExecRecipe()
    //! [5]
    setMFC1();// TODO: need to implement
    MFC1ActualFlow();// TODO: need to implement
    //! [6]
    setMFC2();// TODO: need to implement
    MFC2ActualFlow();// TODO: need to implement
    //! [7]
    setMFC3();// TODO: need to implement
    MFC3ActualFlow();// TODO: need to implement
    //! [8]
    setMFC4();// TODO: need to implement
    MFC4ActualFlow();// TODO: need to implement
    //! [9]
    setTempValue();// TODO: need to implement
    getHeadTemp();// TODO: need to implement
    //! [10]
    //UpdateHandshakeStatus();*/
    //! [11]
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
    ui->axisstatus_dup->setText(status);
    ui->axisstatus_2_dup->setText(next);
}

void MainWindow::recipeExecutionStateChanged(bool state)
{
    if (state == true) {
        ui->plsmaBtn->setText("PLASMA OFF");
        ui->plsmaBtn_dup->setText("PLASMA OFF");
    }
    else {
        ui->plsmaBtn->setText("START PLASMA");
        ui->plsmaBtn_dup->setText("START PLASMA");
    }
}

void MainWindow::SSM_StatusUpdate(QString status, QString next)
{
    ui->axisstatus->setText(status);
    ui->axisstatus_dup->setText(next);
    ui->axisstatus_2->setText(status);
    ui->axisstatus_2_dup->setText(next);
}

//////////////////////////////////////////////////////////////////////////////////
// 3 axis
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::pinsStateChanged(bool state)
{
    if (state) {
        ui->Stagepins_button_dup->setText("PINS OFF");
        ui->Stagepins_button->setText("PINS OFF");
    }
    else {
        ui->Stagepins_button_dup->setText("PINS");
        ui->Stagepins_button->setText("PINS");
    }
}

void MainWindow::joystickStateChanged(bool state)
{
    if (state) {
        ui->Joystick_button_dup->setText("JOY OFF");
        ui->Joystick_button->setText("JOY OFF");
    }
    else {
        ui->Joystick_button_dup->setText("JOY");
        ui->Joystick_button->setText("JOY");
    }
}
void MainWindow::n2StateChanged(bool state)
{
    if (state) {
        ui->n2_purge_button_dup->setText("N2 OFF");
        ui->n2_purge_button->setText("N2 OFF");
    }
    else {
        ui->n2_purge_button_dup->setText("N2 PURGE");
        ui->n2_purge_button->setText("N2 PURGE");
    }
}
void MainWindow::vacStateChanged(bool state)
{
    if (state) {
        ui->vac_button_dup->setText("VAC OFF");
        ui->vac_button->setText("VAC OFF");
    }
    else {
        ui->vac_button_dup->setText("VAC");
        ui->vac_button->setText("VAC");
    }

}

void MainWindow::stageStatusUpdate(QString statusNow, QString statusNext)
{
    // dashboard
    ui->axisstatus->setText(statusNow);
    ui->axisstatus_2->setText(statusNext);

    // 3 axis tab
    ui->axisstatus_dup->setText(statusNow);
    ui->axisstatus_2_dup->setText(statusNext);
}

void MainWindow::AxisStatusToUI()
{
    // XAxis
    if (m_mainCTL.getAxesController().getXAxisState() >= AXIS_IDLE) {
        double Xpos = m_mainCTL.getAxesController().getXPosition();
        ui->X_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordXBase2PH(Xpos)));
        ui->X_relative_PH_dup->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordXBase2PH(Xpos)));
    }
    else {
        ui->X_relative_PH->setText("???");
        ui->X_relative_PH_dup->setText("???");
    }
    // YAxis
    if (m_mainCTL.getAxesController().getYAxisState() >= AXIS_IDLE) {
        double Ypos = m_mainCTL.getAxesController().getYPosition();
        ui->Y_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordYBase2PH(Ypos)));
        ui->Y_relative_PH_dup->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordYBase2PH(Ypos)));
    }
    else {
        ui->Y_relative_PH->setText("???");
        ui->Y_relative_PH_dup->setText("???");
    }
    // ZAxis
    if (m_mainCTL.getAxesController().getZAxisState() >= AXIS_IDLE) {
        double Zpos = m_mainCTL.getAxesController().getZPosition();
        ui->Z_relative_PH->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordZBase2PH(Zpos)));
        ui->Z_relative_PH_dup->setText(QString::number(m_mainCTL.getAxesController().TranslateCoordZBase2PH(Zpos)));
    }
    else {
        ui->Z_relative_PH->setText("???");
        ui->Z_relative_PH_dup->setText("???");
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
        if (!m_mainCTL.m_mfcs.isEmpty()) {
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

    // Get the current directory
    QString currentDirectory = QCoreApplication::applicationDirPath();
    QString initialDirectory = currentDirectory + "/recipes/";

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

    } else {
        // User canceled the file dialog
        qDebug() << "File selection canceled.";
    }
}

void MainWindow::saveRecipe() {
    // Create the directory path
    QString directoryPath = QCoreApplication::applicationDirPath() + "/Recipes";

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
            qDebug() << "Recipe saved to file: " << filePath;
       } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
       }
    }
}


// update the recipe progress bar and value
void MainWindow::updateRecipeFlow(const int mfcNumber, const double recipeFlow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(1)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas1_sliderBar->setValue(int(percentage));
       ui->gas1_sliderbar_dup->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc1_recipe->setText(QString::number(recipeFlow));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas1_recipe_SLPM->setText(QString::number(percentage));
       ui->gas1_recipe_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 2) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas2_sliderBar->setValue(int(percentage));
       ui->gas2_sliderbar_dup->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc2_recipe->setText(QString::number(recipeFlow));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas2_recipe_SLPM->setText(QString::number(percentage));
       ui->gas2_recipe_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 3) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas3_sliderBar->setValue(int(percentage));
       ui->gas3_sliderbar_dup->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc3_recipe->setText(QString::number(recipeFlow));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas3_recipe_SLPM->setText(QString::number(percentage));
       ui->gas3_recipe_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 4) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       int percentage = 0;
       if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
       ui->gas4_sliderBar->setValue(int(percentage));
       ui->gas4_sliderbar_dup->setValue(int(percentage));

       // set dashboard and plasma tab recipe edit box
       ui->mfc4_recipe->setText(QString::number(recipeFlow));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas4_recipe_SLPM->setText(QString::number(percentage));
       ui->gas4_recipe_SLPM_dup->setText(QString::number(percentage));
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
    // update 3 axis tab
    ui->input_thickness_dup->setText(m_mainCTL.getRecipe()->getThicknessQStr());
}

// update recipe gap
void MainWindow::gapChanged()
{
    // update dashboard
    ui->gap_recipe->setText(m_mainCTL.getRecipe()->getGapQStr());
    // update 3 axis tab
    ui->input_gap_dup->setText(m_mainCTL.getRecipe()->getGapQStr());
}

// update recipe overlap
void MainWindow::overlapChanged()
{
    // update dashboard
    ui->overlap_recipe->setText(m_mainCTL.getRecipe()->getOverlapQStr());
    // update 3 axis tab
    ui->input_overlap_dup->setText(m_mainCTL.getRecipe()->getOverlapQStr());
}

// update recipe speed
void MainWindow::speedChanged()
{
    // update dashboard
    ui->speed_recipe->setText(m_mainCTL.getRecipe()->getSpeedQStr());
    // update 3 axis tab
    ui->input_speed_dup->setText(m_mainCTL.getRecipe()->getSpeedQStr());
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

    // update 3 axis
    ui->xmin_controls_dup->setText(m_mainCTL.getRecipe()->getXminPHQStr());
    ui->xmax_controls_dup->setText(m_mainCTL.getRecipe()->getXmaxPHQStr());
}

// update recipe y limits
void MainWindow::yLimitsChanged()
{
    // update dashboard
    ui->y1_recipe->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->y2_recipe->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());

    // update 3 axis
    ui->ymin_controls_dup->setText(m_mainCTL.getRecipe()->getYminPHQStr());
    ui->ymax_controls_dup->setText(m_mainCTL.getRecipe()->getYmaxPHQStr());
}

// update recipe cycles
void MainWindow::cyclesChanged()
{
    // update dashboard
    ui->cycles_recipe->setText(m_mainCTL.getRecipe()->getCyclesQStr());

    // update 3 axis
    ui->input_cycles_dup->setText(m_mainCTL.getRecipe()->getCyclesQStr());
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
    // plasma tab
    ui->RF_Actual_LCD_dup->display(watts);
    ui->RF_bar_dup->setValue(watts);
}

void MainWindow::reflectedWattsChanged()
{
    int watts = m_mainCTL.getPower().getReflectedWatts();

    // dashboard
    ui->RefRF_Actual_LCD->display(watts);
    ui->RefRF_bar->setValue(watts);
    // plasma tab
    ui->RefRF_Actual_LCD_dup->display(watts);
    ui->RefRF_bar_dup->setValue(watts);
}

void MainWindow::MBactualPositionChanged(const double actualPosition)
{
    // dashboard
    ui->MB_Actual_LCD->display(actualPosition);
    ui->MB_Pos_Bar->setValue(int(actualPosition));
    // plasma tab
    ui->MB_Actual_LCD_dup->display(actualPosition);
    ui->MB_Pos_Bar_dup->setValue(int(actualPosition));
}

void MainWindow::headTemperatureChanged()
{
    double temp = m_mainCTL.getPlasmaHead().getTemperature();

    // dashboard
    ui->temp_LCD->display(temp);
    ui->Temp_bar->setValue(int(temp));
    // plasma tab
    ui->temp_LCD_dup->display(temp);
    ui->Temp_bar_dup->setValue(int(temp));
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
       ui->gas1_progressbar_dup->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas1_actual_SLPM->setText(QString::number(percentage));
       ui->gas1_actual_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 2) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas2ProgressBar->setValue(int(percentage));
       ui->gas2_progressbar_dup->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas2_actual_SLPM->setText(QString::number(percentage));
       ui->gas2_actual_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 3) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas3ProgressBar->setValue(int(percentage));
       ui->gas3_progressbar_dup->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas3_actual_SLPM->setText(QString::number(percentage));
       ui->gas3_actual_SLPM_dup->setText(QString::number(percentage));
    }
    else if (mfcNumber == 4) {
       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       int percentage = 0;
       if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection
       ui->gas4ProgressBar->setValue(int(percentage));
       ui->gas4_progressbar_dup->setValue(int(percentage));

       // set the dashboard and plasma tab edit box below the progress bar
       ui->gas4_actual_SLPM->setText(QString::number(percentage));
       ui->gas4_actual_SLPM_dup->setText(QString::number(percentage));
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

// init button on 3 axis tab
void MainWindow::on_init_button_dup_clicked()
{
    m_mainCTL.getAxesController().StartInit();
}

// home button on dash
void MainWindow::on_Home_button_toggled(bool checked)
{
    on_Home_button_dup_toggled(checked);
}

// home button on 3 axis tab
void MainWindow::on_Home_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().StartHome();
    }
    else {
        m_mainCTL.getAxesController().StopHome();
    }
}

// two spot on dashboard
void MainWindow::on_twospot_button_toggled(bool checked)
{
    on_twospot_button_dup_toggled(checked);
}

// two spot on 3 axis tab
void MainWindow::on_twospot_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().StartTwoSpot();
    }
    else {
        m_mainCTL.getAxesController().StopTwoSpot();
    }
}

// scan button on 3 axis tab
void MainWindow::on_scan_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.StartScan();
    }
    else {
        m_mainCTL.StopScan();
    }
}

// scan button on dashboard
void MainWindow::on_scan_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.StartScan();
    }
    else {
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

// pins button on 3 axis tab
void MainWindow::on_Stagepins_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().togglePinsOn();
    }
    else {
        m_mainCTL.getAxesController().togglePinsOff();
    }
}

// n2 purge button on 3 axis tab
void MainWindow::on_n2_purge_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleN2PurgeOn();
    }
    else {
        m_mainCTL.getAxesController().toggleN2PurgeOff();

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
        m_mainCTL.getAxesController().toggleJoystickOff();
    }
}

// joystick button on 3 axis
void MainWindow::on_Joystick_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleJoystickOn();
    }
    else {
        m_mainCTL.getAxesController().toggleJoystickOff();
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
    ui->wafer_diameter_dup->setCurrentIndex(comboboxCurrentIndex);

    // reset the scan box
    m_mainCTL.runDiameter();
}

// diameter button on 3 axis tab
void MainWindow::on_diameter_button_dup_clicked()
{
    // reset the scan box
    m_mainCTL.runDiameter();
}

// wafer combo box on the dashboard
void MainWindow::on_wafer_diameter_currentIndexChanged(int index)
{
    // get the index of the current displayed diameter
    int comboboxCurrentIndex = ui->wafer_diameter->currentIndex();

    // get the current diameter
    int currentDiameter = m_mainCTL.getDiameter().getCurrentWaferDiameterSelection();

    // proposed diameter
    int proposedDiameter = m_mainCTL.getDiameter().getWaferDiameterByIndex(comboboxCurrentIndex);

    if (currentDiameter != proposedDiameter) {

        // set our diameter object
        m_mainCTL.getDiameter().setCurrentWaferDiameter(m_mainCTL.getDiameter().getWaferDiameterByIndex(comboboxCurrentIndex));

        // update the combo box on the dashboard tab
        ui->wafer_diameter_dup->setCurrentIndex(comboboxCurrentIndex);
    }
}


// wafer combo box on the 3 axis page
void MainWindow::on_wafer_diameter_dup_currentIndexChanged(int index)
{
    // get the index of the current displayed diameter
    int comboboxCurrentIndex = ui->wafer_diameter_dup->currentIndex();

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


// vacuum button on 3 axis tab
void MainWindow::on_vac_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.getAxesController().toggleVacOn();
    }
    else {
        m_mainCTL.getAxesController().toggleVacOff();
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
    if (checked) {
        m_mainCTL.StopScan();
    }
    else {
        m_mainCTL.StartScan();
    }
}

// plasma button on plasma tab
void MainWindow::on_plsmaBtn_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.StopScan();
    }
    else {
        m_mainCTL.StartScan();
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
    double doubVal = QInputDialog::getDouble(this, "Speed: ","mm: " + m_mainCTL.getAxesController().getXMaxSpeedQStr(), 0, 0,
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


void MainWindow::on_Y1_set_clicked()
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


void MainWindow::on_Y2_set_clicked()
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


void MainWindow::on_gas1_setpoint_button_dup_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Gas setpoint", "Please enter a setpoint", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double gas1 = input.toDouble();
        m_mainCTL.getMFCs()[0]->setRecipeFlow(gas1);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::on_gas2_setpoint_button_dup_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Gas setpoint", "Please enter a setpoint", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double gas2 = input.toDouble();
        m_mainCTL.getMFCs()[1]->setRecipeFlow(gas2);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::on_gas3_setpoint_button_dup_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Gas setpoint", "Please enter a setpoint", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double gas2 = input.toDouble();
        m_mainCTL.getMFCs()[2]->setRecipeFlow(gas2);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::on_gas4_setpoint_button_dup_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(nullptr, "Gas setpoint", "Please enter a setpoint", QLineEdit::Normal, "", &ok);

    if (ok && !input.isEmpty()) {
        double gas4 = input.toDouble();
        m_mainCTL.getMFCs()[3]->setRecipeFlow(gas4);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

void MainWindow::on_actionEngineer_Mode_triggered()
{
    m_passDialog.show();
}


void MainWindow::on_actionOperator_Mode_triggered()
{

}

