#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include "axiscontroller.h"

int SM_PollCounter = 0;
const int SM_POLL_PERIOD = 5;
std::chrono::milliseconds CTLResetTimeOut = 0ms;


MainWindow::MainWindow(MainLoop* loop, QWidget *parent) :
    QMainWindow(parent),
    m_pMainLoop(loop),
    ui(new Ui::MainWindow),
    m_pStatus(new QLabel),
    m_pSettings(new SettingsDialog),
    m_mainCTL(),
    m_commandFileReader(),
    m_pMainCTLConsole(),
    //m_pStageCTLConsole(),
    m_pStageWidget(new StageWidget(this)),
    m_config(),
    m_waferDiameter()
{    
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE v" + QString(APP_VERSION));

    // setup the state machine
    setupMainStateMachine();

    // ui updates from various sources
    connect(&m_mainCTL.getAxesController(), &AxesController::stageStatusUpdate, this, &MainWindow::stageStatusUpdate);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMStartup, this, &MainWindow::homeStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIHomeSMDone, this, &MainWindow::homeStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIInitSMStartup, this, &MainWindow::initStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUIInitSMDone, this, &MainWindow::initStateMachineDone);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMStartup, this, &MainWindow::twoSpotStateMachineStartup);
    connect(&m_mainCTL.getAxesController(), &AxesController::setUITwoSpotSMDone, this, &MainWindow::twoSpotStateMachineDone);
    connect(&m_mainCTL, &PlasmaController::setRecipeMBtuner, this, &MainWindow::setRecipeMBtuner);
    connect(&m_mainCTL, &PlasmaController::setRecipeRFpower, this, &MainWindow::setRecipeRFpower);
    connect(&m_mainCTL, &PlasmaController::MFC4RecipeFlow, this, &MainWindow::MFC4RecipeFlow);
    connect(&m_mainCTL, &PlasmaController::MFC3RecipeFlow, this, &MainWindow::MFC3RecipeFlow);
    connect(&m_mainCTL, &PlasmaController::MFC2RecipeFlow, this, &MainWindow::MFC2RecipeFlow);
    connect(&m_mainCTL, &PlasmaController::MFC1RecipeFlow, this, &MainWindow::MFC1RecipeFlow);
    connect(&m_mainCTL, &PlasmaController::plasmaHeadTemp, this, &MainWindow::plasmaHeadTemp);
    connect(m_pMainLoop, &MainLoop::runMainStateMachine, this, &MainWindow::runMainStateMachine);


    // TODO: create stage area for custom pathing
//    ui->verticalLayout_4->addWidget(m_pStageWidget);
//    m_pStageWidget->setStageBounds(0.0, 100.0, 0.0, 50.0);

    // Make signal/slot connections here
    connectRecipeButtons(); // TODO: remove these and replace with designer click handlers
    connectCascadeRecipeButtons(); // TODO: remove these and replace with designer click handlers

    // recipe slots
    connectMFCFlowBars();
    connect(&m_mainCTL.m_pwr, &PWR::recipeWattsChanged, this, &MainWindow::recipeWattsChanged);

    // status bar
    ui->statusBar->addWidget(m_pStatus);

    // setup wafer diamter combo box
    ui->wafer_diameter->addItems(m_waferDiameter.getWaferDiameterTextList());
    ui->wafer_diameter_dup->addItems(m_waferDiameter.getWaferDiameterTextList());

    //this->openMainPort();
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
    //int tabIndex = ui->mainTabWidget->addTab(m_pMainCTLConsole, "Main CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    //ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(m_pMainCTLConsole, &Console::getData, this, &MainWindow::writeMainPort);
}


void MainWindow::connectRecipeButtons()
{
    connect(ui->loadMBButton, &QPushButton::clicked, this, &MainWindow::loadMBRecipeButton_clicked);
    //connect(ui->AutoTuneCheckBox, &QCheckBox::stateChanged, this, &MainWindow::AutoTuneCheckbox_stateChanged); // MCD: should this be a checkbox?

    //MFC buttons
    connectMFCRecipeButton(ui->loadMFC1Button, 1);
    connectMFCRecipeButton(ui->loadMFC2Button, 2);
    connectMFCRecipeButton(ui->loadMFC3Button, 3);
    connectMFCRecipeButton(ui->loadMFC4Button, 4);
}

void MainWindow::connectCascadeRecipeButtons()
{
    connect(ui->addCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::addRecipeToCascadeRecipe);
    connect(ui->removeCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::removeRecipeFromCascadeList);
    connect(ui->saveAsCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::saveAsCascadeRecipeListToFile);
}

void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < m_mainCTL.m_mfcs.size(); ++i) {
        connect(m_mainCTL.m_mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeFlow);
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
    m_mainCTL.AxisStartup();
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
    //    RunScanBtn.Visible = False
    //    SetTwoSpotBtn.Visible = False
    //    SetDiameterBtn.Visible = False
    //    HomeAxesBtn.Text = "STOP"
    //            PinsSquare.BackColor = Color.Gainsboro
    //            b_HasPins = False 'This is so the first time the button is hit, the button will bury the pins

    ui->twospot_button->setEnabled(false);
    ui->twospot_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
    ui->Home_button->setText("STOP");
    ui->Home_button_dup->setText("STOP");
}

// set ui elements accordingly
void MainWindow::homeStateMachineDone()
{
    ui->twospot_button->setEnabled(true);
    ui->twospot_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
    ui->Home_button->setText("LOAD");
    ui->Home_button_dup->setText("LOAD");
    ui->Home_button->setChecked(false);
    ui->Home_button_dup->setChecked(false);
}

// set ui elements accordingly
void MainWindow::initStateMachineStartup()
{
    //GUI status
    //ui->Stagepins_button->setChecked(true); // TODO: need to implement
    //RunScanBtn.Visible = False
    ui->twospot_button->setEnabled(false);
    ui->twospot_button_dup->setEnabled(false);
    ui->diameter_button->setEnabled(false);
    ui->diameter_button_dup->setEnabled(false);
    ui->Home_button->setEnabled(false);
    ui->Home_button_dup->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->init_button_dup->setEnabled(false);
}

// set ui elements accordingly
void MainWindow::initStateMachineDone()
{
    //    RunScanBtn.Visible = True
    //    Vacbtn.Visible = True
    //    RecipeButtonPins.Visible = True
    //    AutoVacSquare.Visible = True
    //    PinsSquare.Visible = True
    //    PinsSquare.BackColor = Color.Lime
    //    b_HasPins = True 'This is so the first time the button is hit, the button will bury the pins
    //    If b_ENG_mode Then
    //        SetTwoSpotBtn.Visible = True
    //        SetDiameterBtn.Visible = True
    //    End If
    ui->twospot_button->setEnabled(true);
    ui->twospot_button_dup->setEnabled(true);
    ui->diameter_button->setEnabled(true);
    ui->diameter_button_dup->setEnabled(true);
    ui->Home_button->setEnabled(true);
    ui->Home_button_dup->setEnabled(true);
    ui->init_button->setEnabled(true);
    ui->init_button_dup->setEnabled(true);
    ui->init_button->setChecked(false);
    ui->init_button_dup->setChecked(false);
}

void MainWindow::twoSpotStateMachineStartup()//    m_scanStateMachine.addState(m_pScanState);
{
    ui->twospot_button->setText("STOP");

    ui->Home_button->setEnabled(false);
    ui->Home_button_dup->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->init_button_dup->setEnabled(false);
    ui->init_button->setChecked(false);
    ui->init_button_dup->setChecked(false);
}

void MainWindow::twoSpotStateMachineDone()
{
    ui->twospot_button->setText("TWO SPOT");

    // 3 axis tab
    ui->xmin_controls_dup->setText(QString::number(m_mainCTL.getXTwoSpotFirstPoint()));
    ui->xmax_controls_dup->setText(QString::number(m_mainCTL.getXTwoSpotSecondPoint()));
    ui->ymin_controls_dup->setText(QString::number(m_mainCTL.getYTwoSpotFirstPoint()));
    ui->ymax_controls_dup->setText(QString::number(m_mainCTL.getYTwoSpotSecondPoint()));

    // dashboard tab
    ui->x1_recipe->setText(QString::number(m_mainCTL.getXTwoSpotFirstPoint()));
    ui->x2_recipe->setText(QString::number(m_mainCTL.getXTwoSpotSecondPoint()));
    ui->y1_recipe->setText(QString::number(m_mainCTL.getYTwoSpotFirstPoint()));
    ui->y2_recipe->setText(QString::number(m_mainCTL.getYTwoSpotSecondPoint()));

    ui->Home_button->setEnabled(true);
    ui->Home_button_dup->setEnabled(true);
    ui->init_button->setEnabled(true);
    ui->init_button_dup->setEnabled(true);
    ui->twospot_button->setChecked(false);
    ui->twospot_button->setChecked(false);
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

        //Terminal Tab setup for console commands
        consoleMainCTLSetup();
        m_pMainCTLConsole->setEnabled(true);
        m_pMainCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        m_mainCTL.resetAxes();
        //resetCTL(); // TODO:  Needs implementing
        CTLResetTimeOut = 2500ms / m_pMainLoop->getTimerInterval();
        //(DEBUG_MODE) ? MainStateMachine.setState(IDLE) : MainStateMachine.setState(STARTUP); // TODO: Needs implementing

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
        //RunCheckInput(); TODO: Need to implement

        SM_PollCounter += 1;
        if (SM_PollCounter >= SM_POLL_PERIOD) {
            SM_PollCounter = 0;
            RunPolling();
            UpdateStatus();
            // setLightTower(); TODO: Need to implement
            m_mainCTL.RunInitAxesSM();
            m_mainCTL.RunTwoSpotSM();
            m_mainCTL.RunHomeAxesSM();
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
    m_mainCTL.getAxisStatus();
    AxisStatusToUI();
}

//////////////////////////////////////////////////////////////////////////////////
// Plasma controller
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::setRecipeMBtuner(QString MBtunerSP)
{
    ui->mb_recipe->setText(MBtunerSP);
}

void MainWindow::setRecipeRFpower(QString RFpowerSP)
{
    ui->RF_recipe_watts->setText(RFpowerSP);
}

void MainWindow::MFC4RecipeFlow(QString recipeFlow)
{
    ui->gas4_recipe_SLPM->setText(recipeFlow);
    ui->mfc4_recipe->setText(recipeFlow);

    // set vertical progress bar
    double range = m_mainCTL.findMFCByNumber(4)->getRange();
    double flow = m_mainCTL.findMFCByNumber(4)->getRecipeFlow();
    double percentage = 0;
    if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
    ui->gas4_sliderBar->setValue(int(percentage));
}

void MainWindow::MFC3RecipeFlow(QString recipeFlow)
{
    ui->gas3_recipe_SLPM->setText(recipeFlow);
    ui->mfc3_recipe->setText(recipeFlow);

    // set vertical progress bar
    double range = m_mainCTL.findMFCByNumber(3)->getRange();
    double flow = m_mainCTL.findMFCByNumber(3)->getRecipeFlow();
    double percentage = 0;
    if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
    ui->gas3_sliderBar->setValue(int(percentage));
}

void MainWindow::MFC2RecipeFlow(QString recipeFlow)
{
    ui->gas4_recipe_SLPM->setText(recipeFlow);
    ui->mfc4_recipe->setText(recipeFlow);

    // set vertical progress bar
    double range = m_mainCTL.findMFCByNumber(2)->getRange();
    double flow = m_mainCTL.findMFCByNumber(2)->getRecipeFlow();
    double percentage = 0;
    if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
    ui->gas2_sliderBar->setValue(int(percentage));
}

void MainWindow::MFC1RecipeFlow(QString recipeFlow)
{
    ui->gas4_recipe_SLPM->setText(recipeFlow);
    ui->mfc4_recipe->setText(recipeFlow);

    // set vertical progress bar
    double range = m_mainCTL.findMFCByNumber(1)->getRange();
    double flow = m_mainCTL.findMFCByNumber(1)->getRecipeFlow();
    double percentage = 0;
    if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
    ui->gas1_sliderBar->setValue(int(percentage));
}

void MainWindow::plasmaHeadTemp(double temp)
{
    ui->temp_LCD->display(temp);
    ui->Temp_bar->setValue(int(temp));
}

//////////////////////////////////////////////////////////////////////////////////
// 3 axis
//////////////////////////////////////////////////////////////////////////////////
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
    if (m_mainCTL.getXAxisState() >= AXIS_IDLE) {
        double Xpos = m_mainCTL.getXPosition();
        ui->X_relative_PH->setText(QString::number(m_mainCTL.TranslateCoordXPH2Base(Xpos)));
        ui->X_relative_PH_dup->setText(QString::number(m_mainCTL.TranslateCoordXPH2Base(Xpos)));
    }
    else {
        ui->X_relative_PH->setText("???");
        ui->X_relative_PH_dup->setText("???");
    }
    // YAxis
    if (m_mainCTL.getYAxisState() >= AXIS_IDLE) {
        double Ypos = m_mainCTL.getYPosition();
        ui->Y_relative_PH->setText(QString::number(m_mainCTL.TranslateCoordYPH2Base(Ypos)));
        ui->Y_relative_PH_dup->setText(QString::number(m_mainCTL.TranslateCoordYPH2Base(Ypos)));
    }
    else {
        ui->Y_relative_PH->setText("???");
        ui->Y_relative_PH_dup->setText("???");
    }
    // ZAxis
    if (m_mainCTL.getZAxisState() >= AXIS_IDLE) {
        double Zpos = m_mainCTL.getZPosition();
        ui->Z_relative_PH->setText(QString::number(m_mainCTL.TranslateCoordZPH2Base(Zpos)));
        ui->Z_relative_PH_dup->setText(QString::number(m_mainCTL.TranslateCoordZPH2Base(Zpos)));
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
    m_mainCTL.m_tuner.setAutoTune(value);
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

       // Open the file for writing
       QFile file(filePath);
       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            QMap<QString, QVariant> recipe = m_mainCTL.getRecipeMap();
            // Write each recipe name to the file
            for (auto it = recipe.begin(); it != recipe.end(); it++) {
                const QString& key = it.key();
                const QVariant& value = it.value();

                out << key << "=" << value.toString() << "\n";
            }

            file.close();
            qDebug() << "Recipe saved to file: " << filePath;
       } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
       }
    }
}

void MainWindow::openCascadeRecipe()
{
    //  plasmaRecipe.currentRecipeIndex_;
}

void MainWindow::addRecipeToCascadeRecipe()
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
       m_mainCTL.addRecipeToCascade(fileName);

       // Update the UI with the recipes
       ui->listCascadeRecipes->addItem(fileName);
    } else {
       // User canceled the file dialog
       qDebug() << "File selection canceled.";
    }
}

void MainWindow::removeRecipeFromCascadeList()
{
    // Get the selected item in the list widget
    QListWidgetItem* selectedItem = ui->listCascadeRecipes->currentItem();
    if (selectedItem) {
       // Get the text of the selected item
       QString recipeFileName = selectedItem->text();

       // Remove the item from the list widget
       ui->listCascadeRecipes->takeItem(ui->listCascadeRecipes->row(selectedItem));

       // Remove the item from the cascade recipe list
       m_mainCTL.removeRecipeFromCascade(recipeFileName);
    }

}

void MainWindow::saveAsCascadeRecipeListToFile() {
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
            for (const QString& recipeName : m_mainCTL.getCascadeRecipeList()) {
                out << recipeName << "\n";
            }

            file.close();
            qDebug() << "Cascade recipe list saved to file: " << filePath;
       } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
       }
    }
}

// update the recipe progress bar and value
void MainWindow::updateRecipeFlow(const int& mfcNumber, const double& flow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {

       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(1)->getRange();
       double percentage = 100;
       if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
       ui->gas1_sliderBar->setValue(int(percentage));

       // set dashboard recipe edit box
       ui->mfc1_recipe->setText(QString::number(percentage));

       // set the dashboard edit box below the progress bar
       ui->gas1_recipe_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 2) {

       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(2)->getRange();
       double percentage = 100;
       if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
       ui->gas2_sliderBar->setValue(int(percentage));

       // set dashboard recipe edit box
       ui->mfc2_recipe->setText(QString::number(percentage));

       // set the dashboard edit box below the progress bar
       ui->gas2_recipe_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 3) {

       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(3)->getRange();
       double percentage = 100;
       if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
       ui->gas3_sliderBar->setValue(int(percentage));

       // set dashboard recipe edit box
       ui->mfc3_recipe->setText(QString::number(percentage));

       // set the dashboard edit box below the progress bar
       ui->gas3_recipe_SLPM->setText(QString::number(percentage));
    }
    else if (mfcNumber == 4) {

       // set vertical progress bar
       double range = m_mainCTL.findMFCByNumber(4)->getRange();
       double percentage = 100;
       if (range != 0) percentage = (flow / range) * 100.0; // divide by zero protection
       ui->gas4_sliderBar->setValue(int(percentage));

       // set dashboard recipe edit box
       ui->mfc4_recipe->setText(QString::number(percentage));

       // set the dashboard edit box below the progress bar
       ui->gas4_recipe_SLPM->setText(QString::number(percentage));
    }
}

// update the recipe watts
void MainWindow::recipeWattsChanged()
{
    int watts = m_mainCTL.m_pwr.getRecipeWatts();
    ui->RF_recipe_watts->setText(QString::number(watts));
}


//////////////////////////////////////////////////////////////////////////////////
// Button handlers
//////////////////////////////////////////////////////////////////////////////////

void MainWindow::loadMBRecipeButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "Tuner Setpoint", "Please enter a setpoint for MB Tuner:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
       // User entered a string and clicked OK
       double recipe = recipeStr.toDouble();
       m_mainCTL.m_tuner.setRecipePosition(recipe);
    }
    else {
       // User either clicked Cancel or did not enter any string
       // Handle accordingly
       return;
    }
}

// init button on dash
void MainWindow::on_init_button_clicked()
{
    m_mainCTL.StartInit();
}

// init button on 3 axis tab
void MainWindow::on_init_button_dup_clicked()
{
    m_mainCTL.StartInit();
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
        m_mainCTL.StartHome();
    }
    else {
        m_mainCTL.StopHome();
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
        m_mainCTL.StartTwoSpot();
    }
    else {
        m_mainCTL.StopTwoSpot();
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

// pins button on 3 axis tab
void MainWindow::on_Stagepins_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.togglePinsOn();

        // TODO
        ui->Stagepins_button_dup->setText("PINS OFF");
    }
    else {
        m_mainCTL.togglePinsOff();

        // TODO
        ui->Stagepins_button_dup->setText("PINS");
    }
}

// n2 purge button on 3 axis tab
void MainWindow::on_n2_purge_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.toggleN2PurgeOn();

        // TODO
        ui->n2_purge_button_dup->setText("N2 OFF");
    }
    else {
        m_mainCTL.toggleN2PurgeOff();

        // TODO
        ui->n2_purge_button_dup->setText("N2 PURGE");
    }
}

// n2 purge button on dashboard
void MainWindow::on_n2_purge_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.toggleN2PurgeOn();

        // TODO
        ui->n2_purge_button->setText("N2 OFF");
    }
    else {
        m_mainCTL.toggleN2PurgeOff();

        // TODO
        ui->n2_purge_button->setText("N2 PURGE");
    }

}

// diameter button on 3 axis tab
void MainWindow::on_diameter_button_dup_clicked()
{

}

void MainWindow::on_Joystick_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.toggleJoystickOn();

        // TODO
        ui->Joystick_button_dup->setText("JOY OFF");
    }
    else {
        m_mainCTL.toggleJoystickOff();

        // TODO
        ui->Joystick_button_dup->setText("JOY");
    }
}

// wafer combo box on the 3 axis page
void MainWindow::on_wafer_diameter_dup_currentIndexChanged(int index)
{
    m_waferDiameter.setCurrentWaferDiameter(m_waferDiameter.getWaferDiameterByIndex(index));
}

// vacuum button on 3 axis tab
void MainWindow::on_vac_button_dup_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.toggleVacOn();

        // TODO
        ui->vac_button_dup->setText("VAC OFF");
    }
    else {
        m_mainCTL.toggleVacOff();

        // TODO
        ui->vac_button_dup->setText("VAC");
    }
}

// vacuum button on dashboard
void MainWindow::on_vac_button_toggled(bool checked)
{
    if (checked) {
        m_mainCTL.toggleVacOn();
    }
    else {
        m_mainCTL.toggleVacOff();
    }
}

void MainWindow::on_load_thick_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Thickness: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.setRecipeThickness(doubVal);
        // update dashboard
        ui->thickness_recipe->setText(m_mainCTL.getRecipeThicknessQStr());
        // update 3 axis tab
        ui->input_thickness_dup->setText(m_mainCTL.getRecipeThicknessQStr());
    }
}


void MainWindow::on_load_gap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Gap: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.setRecipeGap(doubVal);
        // update dashboard
        ui->gap_recipe->setText(m_mainCTL.getRecipeGapQStr());
        // update 3 axis tab
        ui->gap_recipe->setText(m_mainCTL.getRecipeGapQStr());
    }
}


void MainWindow::on_load_overlap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Overlap: ","mm: ", 0, 0, 5.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.setRecipeOverlap(doubVal);
        // update dashboard
        ui->overlap_recipe->setText(m_mainCTL.getRecipeOverlapQStr());
        // update 3 axis tab
        ui->input_overlap_dup->setText(m_mainCTL.getRecipeOverlapQStr());
    }
}


void MainWindow::on_loadSpeedButton_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Speed: ","mm: " + m_mainCTL.getXMaxSpeedQStr(), 0, 0,
                                             m_mainCTL.XMaxSpeed(), 0, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_mainCTL.setRecipeSpeed(doubVal);
        // update dashboard
        ui->speed_recipe->setText(m_mainCTL.getRecipeSpeedQStr());
        // update 3 axis tab
        ui->input_speed_dup->setText(m_mainCTL.getRecipeSpeedQStr());
    }
}


void MainWindow::on_load_cycles_clicked()
{
    bool ok;
    int intVal = QInputDialog::getInt(this, "Number of Cycles","cycles: ", 0, 0, 200, 0, &ok);
    if (ok) {
        m_mainCTL.setRecipeCycles(intVal);
        // update dashboard
        ui->cycles_recipe->setText(m_mainCTL.getRecipeCyclesQStr());
        // update 3 axis tab
        ui->input_cycles_dup->setText(m_mainCTL.getRecipeCyclesQStr());
    }
}


void MainWindow::on_loadRecipeButton_clicked()
{
    openRecipe();
}

void MainWindow::on_loadRFButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "RF Setpoint", "Please enter a setpoint for RF Power:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        int recipe = recipeStr.toInt();
        m_mainCTL.m_pwr.setRecipeWatts(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


