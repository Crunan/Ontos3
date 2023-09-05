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
    m_stageCTL(),
    m_commandFileReader(),
    m_recipe(),
    m_plasmaRecipe(&m_mainCTL),
    m_pMainCTLConsole(),
    m_pStageCTLConsole(),
    m_pStageWidget(new StageWidget(this)),
    m_config(),
    m_waferDiameter()
{    
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE");

    // Signal for Power down button
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);

    // Serial buttons initial states
    serialButtonPreConnectState();

    // Main Serial connect/disconnect buttons
    connect(ui->mainConnectButton, &QPushButton::clicked, this, &MainWindow::openMainPort);
    connect(ui->mainDisconnectButton, &QPushButton::clicked, this, &MainWindow::closeMainPort);
    // Stage Serial connect/disconnect buttons
    connect(ui->stageConnectButton, &QPushButton::clicked, this, &MainWindow::openStagePort);
    //connect(ui->stageDisconnectButton, &QPushButton::clicked, this, &MainWindow::closeStagePort);
    // Stage buttons
    //connect(ui->init_button, &QPushButton::clicked, this, &MainWindow::initButtonClicked);
    // Settings button
    connect(ui->mainSettingsButton, &QPushButton::clicked, m_pSettings, &SettingsDialog::show);
    connect(ui->stageSettingsButton, &QPushButton::clicked, m_pSettings, &SettingsDialog::show);

    // ui updates from various sources
    connect(&m_stageCTL, &AxesController::stageStatusUpdate, this, &MainWindow::stageStatusUpdate);
    connect(&m_stageCTL, &AxesController::stageResponseReceived, this, &MainWindow::stageResponseUpdate);
    connect(&m_stageCTL, &AxesController::setUIHomeSMStartup, this, &MainWindow::homeStateMachineStartup);
    connect(&m_stageCTL, &AxesController::setUIHomeSMDone, this, &MainWindow::homeStateMachineDone);
    connect(&m_stageCTL, &AxesController::setUIInitSMStartup, this, &MainWindow::initStateMachineStartup);
    connect(&m_stageCTL, &AxesController::setUIInitSMDone, this, &MainWindow::initStateMachineDone);
    connect(&m_stageCTL, &AxesController::setUITwoSpotSMStartup, this, &MainWindow::twoSpotStateMachineStartup);
    connect(&m_stageCTL, &AxesController::setUITwoSpotSMDone, this, &MainWindow::twoSpotStateMachineDone);

    connect(m_pMainLoop, &MainLoop::runMainStateMachine, this, &MainWindow::runMainStateMachine);

    // TODO: create stage area for custom pathing
    ui->verticalLayout_4->addWidget(m_pStageWidget);
    m_pStageWidget->setStageBounds(0.0, 100.0, 0.0, 50.0);

    // Make signal/slot connections here
    connectRecipeButtons();
    connectCascadeRecipeButtons();
    connectMFCFlowBars();

    // status bar
    ui->statusBar->addWidget(m_pStatus);

    setupMainStateMachine();

    //this->openMainPort();

    // setup wafer diamter combo box
    ui->wafer_diameter->addItems(m_waferDiameter.getWaferDiameterTextList());
    ui->wafer_diameter_dup->addItems(m_waferDiameter.getWaferDiameterTextList());
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

void MainWindow::openMainPort()
{
    const SettingsDialog::Settings p = m_pSettings->settings();

    if (m_mainCTL.open(*m_pSettings)) {

        //Terminal Tab setup for console commands
        consoleMainCTLSetup();        
        m_pMainCTLConsole->setEnabled(true);
        m_pMainCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        // Update UI buttonsvoid stageStatusUpdate(QString status);
        ui->mainConnectButton->setEnabled(false);
        ui->mainDisconnectButton->setEnabled(true);
        ui->mainSettingsButton->setEnabled(false);

        // Give status on connect
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), m_mainCTL.getPortErrorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::openStagePort()
{
    const SettingsDialog::Settings p = m_pSettings->settings();
    if (m_stageCTL.open(*m_pSettings)) {

        //Terminal Tab setup for console commands
        consoleStageCTLSetup();
        m_pStageCTLConsole->setEnabled(true);
        m_pStageCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        // Update UI buttons
        ui->stageConnectButton->setEnabled(false);
        ui->stageDisconnectButton->setEnabled(true);
        ui->stageSettingsButton->setEnabled(false);

        m_stageCTL.resetAxes();
        //resetCTL(); // TODO:  Needs implementing
        CTLResetTimeOut = 2500ms / m_pMainLoop->getTimerInterval();
        //(DEBUG_MODE) ? MainStateMachine.setState(IDLE) : MainStateMachine.setState(STARTUP); // TODO: Needs implementing

        // start the main state machine
        emit MSM_TransitionStartup();

        Logger::init();

        // Give status on connect
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), m_stageCTL.getPortErrorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeMainPort()
{
    if (m_mainCTL.isOpen()) {
        m_mainCTL.close();
    }

    // Disable Console
    m_pMainCTLConsole->setEnabled(false);

    // Default button states
    ui->mainConnectButton->setEnabled(true);
    ui->mainDisconnectButton->setEnabled(false);
    ui->mainSettingsButton->setEnabled(true);

    // Update Status bar
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::closeStagePort()
{
    if (m_stageCTL.isOpen()) {
        m_stageCTL.close();
    }

    // Disable Console
    m_pStageCTLConsole->setEnabled(false);

    // Default button states
    ui->stageConnectButton->setEnabled(true);
    ui->stageDisconnectButton->setEnabled(false);
    ui->stageSettingsButton->setEnabled(true);

    // Update Status bar
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::writeMainPort(const QByteArray &data)
{
    if (!m_mainCTL.sendCommand(data))
        qDebug() << "Command not sent to main CTL";
}

void MainWindow::writeStagePort(const QByteArray &data)
{
    if (!m_stageCTL.sendCommand(data))
        qDebug() << "Command not sent to stage CTL";
}

QString MainWindow::readMainPort()
{
    return m_mainCTL.readData();
}

QString MainWindow::readStagePort()
{
    return m_stageCTL.readResponse();

    // Update console without side effect
    //stageCTLConsole->putData(data);

}
void MainWindow::handleMainSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_mainCTL.getPortErrorString());
        Logger::logCritical(m_mainCTL.getPortErrorString());
        closeMainPort();
    }
}

void MainWindow::handleStageSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_stageCTL.getPortErrorString());
        Logger::logCritical(m_stageCTL.getPortErrorString());
        closeStagePort();
    }
}

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
}

void MainWindow::twoSpotStateMachineStartup()
{
    ui->twospot_button->setText("STOP");
}

void MainWindow::twoSpotStateMachineDone()
{
    ui->twospot_button->setText("TWO SPOT");

    ui->xmin_controls_dup->setText(QString::number(m_stageCTL.getXTwoSpotFirstPoint()));
    ui->xmax_controls_dup->setText(QString::number(m_stageCTL.getXTwoSpotSecondPoint()));
    ui->ymin_controls_dup->setText(QString::number(m_stageCTL.getYTwoSpotFirstPoint()));
    ui->ymax_controls_dup->setText(QString::number(m_stageCTL.getYTwoSpotSecondPoint()));
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
    /*if (m_mainCTL.isOpen()) {
        m_mainCTL.close();
    }*/ // TODO: Needs implementing
    if (m_stageCTL.isOpen()) {
        m_stageCTL.close();
    }
    Logger::clean();
    MainWindow::close();
}

void MainWindow::consoleMainCTLSetup()
{
    // Step 1: Create an instance of the console class
    m_pMainCTLConsole = new Console(ui->mainTabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->mainTabWidget->addTab(m_pMainCTLConsole, "Main CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(m_pMainCTLConsole, &Console::getData, this, &MainWindow::writeMainPort);
}

void MainWindow::consoleStageCTLSetup()
{
    // Step 1: Create an instance of the console class
    m_pStageCTLConsole = new Console(ui->mainTabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->mainTabWidget->addTab(m_pStageCTLConsole, "Stage CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(m_pStageCTLConsole, &Console::getData, this, &MainWindow::writeStagePort);
}

void MainWindow::connectRecipeButtons()
{
    connect(ui->loadRecipeButton, &QPushButton::clicked, this, &MainWindow::RFRecipeButton_clicked);
    connect(ui->loadMBButton, &QPushButton::clicked, this, &MainWindow::loadMBRecipeButton_clicked);
    //connect(ui->AutoTuneCheckBox, &QCheckBox::stateChanged, this, &MainWindow::AutoTuneCheckbox_stateChanged); // MCD: should this be a checkbox?
    connect(ui->loadRecipeButton, &QPushButton::clicked, this, &MainWindow::openRecipe);
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
    for (int i = 0; i < m_mainCTL.mfcs.size(); ++i) {
        connect(m_mainCTL.mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeProgressBar);
    }
}

void MainWindow::connectMFCRecipeButton(QPushButton* button, const int& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}



void MainWindow::updateRecipeProgressBar(const int& mfcNumber, const double& flow)
{
    // This uses the parameters passed in the signal
    /* MCD: Ask Cory.  Temporary just to compile
     * if (mfcNumber == 1) {
        ui->recipeProgressBar->setValue(flow);
    } else if (mfcNumber == 2) {
        ui->recipeProgressBar_2->setValue(flow);
    } else if (mfcNumber == 3) {
        ui->recipeProgressBar_3->setValue(flow);
    } else if (mfcNumber == 4) {
        ui->recipeProgressBar_4->setValue(flow);
    }*/

    // MCD: Temporary just to compile
    if (mfcNumber == 1) {
        ui->gas1ProgressBar->setValue(flow);
    } else if (mfcNumber == 2) {
        ui->gas2ProgressBar->setValue(flow);
    } else if (mfcNumber == 3) {
        ui->gas3ProgressBar->setValue(flow);
    } else if (mfcNumber == 4) {
        ui->gas4ProgressBar->setValue(flow);
    }
}

void MainWindow::serialButtonPreConnectState()
{
    // Enable serial buttons
    ui->mainConnectButton->setEnabled(true);
    ui->mainDisconnectButton->setEnabled(false);
    ui->mainSettingsButton->setEnabled(true);

    ui->stageConnectButton->setEnabled(true);
    ui->stageDisconnectButton->setEnabled(false);
    ui->stageSettingsButton->setEnabled(true);
}

void MainWindow::openRecipeWindowMFC()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "MFC Setpoint", "Please enter a setpoint for the MFC:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        if (!m_mainCTL.mfcs.isEmpty()) {
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
void MainWindow::RFRecipeButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "RF Setpoint", "Please enter a setpoint for RF Power:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        int recipe = recipeStr.toInt();
        m_mainCTL.pwr.setRecipeWatts(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::loadMBRecipeButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "Tuner Setpoint", "Please enter a setpoint for MB Tuner:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        double recipe = recipeStr.toDouble();
        m_mainCTL.tuner.setRecipePosition(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::AutoTuneCheckbox_stateChanged(int value)
{
    m_mainCTL.tuner.setAutoTune(value);
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

        // set plasma Recipe path and file
        m_plasmaRecipe.fileReader.setFilePath(filePath);
        m_plasmaRecipe.setRecipeFromFile();

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

            QMap<QString, QVariant> recipe = m_plasmaRecipe.getRecipeMap();
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
        m_plasmaRecipe.addRecipeToCascade(fileName);

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
        m_plasmaRecipe.removeRecipeFromCascade(recipeFileName);
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
            for (const QString& recipeName : m_plasmaRecipe.getCascadeRecipeList()) {
                out << recipeName << "\n";
            }

            file.close();
            qDebug() << "Cascade recipe list saved to file: " << filePath;
        } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
        }
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

void MainWindow::stageResponseUpdate(QString status)
{
    ui->textRCVbox->appendPlainText(status);
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
            m_stageCTL.RunInitAxesSM();
            m_stageCTL.RunTwoSpotSM();
            m_stageCTL.RunHomeAxesSM();
            //RunScanAxesSM(); TODO: Need to implement

        }
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainIdleState)) { // in Idle state
    }
    else if (m_mainStateMachine.configuration().contains(m_pMainShutdownState)) { // in Shutdown state
    }
}

void MainWindow::RunStartup()
{
    //GetExeCfg(); TODO: need to implement
    m_mainCTL.CTLStartup(); // TODO: need to implement
    m_stageCTL.AxisStartup();
}

void MainWindow::GetExeCfg()
{
    QStringList Values;
    QFile file("./config/default.cfg");

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        Values += line.split(">");
    }

    file.close();

    loadConfigGUI(Values);
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
    m_stageCTL.getAxisStatus(); // TODO: uncomment
    AxisStatusToUI(); // TODO: uncomment
}

void MainWindow::AxisStatusToUI()
{
    // update current positions on dashboard
    ui->X_relative_PH->setText(QString::number(m_stageCTL.getXPosition()));
    ui->Y_relative_PH->setText(QString::number(m_stageCTL.getYPosition()));
    ui->Z_relative_PH->setText(QString::number(m_stageCTL.getZPosition()));
    // update current positions on 3 axis tab
    ui->X_relative_PH_dup->setText(QString::number(m_stageCTL.getXPosition()));
    ui->Y_relative_PH_dup->setText(QString::number(m_stageCTL.getYPosition()));
    ui->Z_relative_PH_dup->setText(QString::number(m_stageCTL.getZPosition()));
}

void MainWindow::loadConfigGUI(QStringList value)
{
    /* // TODO: need to implement
    config.setMFC1(value[1]);
    ui->gas1_label->setText(config.getMFC1());
    config.setMFC2(value[3]);
    ui->gas2_label->setText(config.getMFC2());
    config.setMFC3(value[5]);
    ui->gas3_label->setText(config.getMFC3());
    config.setMFC4(value[7]);
    ui->gas4_label->setText(config.getMFC4());
*/
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

// init button on dash
void MainWindow::on_init_button_clicked()
{
    m_stageCTL.StartInit();
}

// init button on 3 axis tab
void MainWindow::on_init_button_dup_clicked()
{
    m_stageCTL.StartInit();
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
        m_stageCTL.StartHome();
    }
    else {
        m_stageCTL.StopHome();
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
        m_stageCTL.StartTwoSpot();
    }
    else {
        m_stageCTL.StopTwoSpot();
    }
}

// pins button on 3 axis tab
void MainWindow::on_Stagepins_button_dup_toggled(bool checked)
{
    if (checked) {
        m_stageCTL.togglePinsOn();

        // TODO
        ui->Stagepins_button_dup->setText("PINS OFF");
    }
    else {
        m_stageCTL.togglePinsOff();

        // TODO
        ui->Stagepins_button_dup->setText("PINS");
    }
}

// n2 purge button on 3 axis tab
void MainWindow::on_n2_purge_button_dup_toggled(bool checked)
{
    if (checked) {
        m_stageCTL.toggleN2PurgeOn();

        // TODO
        ui->n2_purge_button_dup->setText("N2 OFF");
    }
    else {
        m_stageCTL.toggleN2PurgeOff();

        // TODO
        ui->n2_purge_button_dup->setText("N2 PURGE");
    }
}

// n2 purge button on dashboard
void MainWindow::on_n2_purge_button_toggled(bool checked)
{
    if (checked) {
        m_stageCTL.toggleN2PurgeOn();

        // TODO
        ui->n2_purge_button->setText("N2 OFF");
    }
    else {
        m_stageCTL.toggleN2PurgeOff();

        // TODO
        ui->n2_purge_button->setText("N2 PURGE");
    }

}

// diameter button on 3 axis tab
void MainWindow::on_diameter_button_dup_clicked()
{

}

// close serial on 3 axis tab
void MainWindow::on_stageDisconnectButton_clicked()
{
    closeStagePort();
}


void MainWindow::on_Joystick_button_dup_toggled(bool checked)
{
    if (checked) {
        m_stageCTL.toggleJoystickOn();

        // TODO
        ui->Joystick_button_dup->setText("JOY OFF");
    }
    else {
        m_stageCTL.toggleJoystickOff();

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
        m_stageCTL.toggleVacOn();

        // TODO
        ui->vac_button_dup->setText("VAC OFF");
    }
    else {
        m_stageCTL.toggleVacOff();

        // TODO
        ui->vac_button_dup->setText("VAC");
    }
}

// vacuum button on dashboard
void MainWindow::on_vac_button_toggled(bool checked)
{
    if (checked) {
        m_stageCTL.toggleVacOn();
    }
    else {
        m_stageCTL.toggleVacOff();
    }
}


void MainWindow::on_load_thick_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Thickness: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_recipe.setThickness(doubVal);
        // update dashboard
        ui->thickness_recipe->setText(m_recipe.getThickness());
        // update 3 axis tab
        ui->input_thickness_dup->setText(m_recipe.getThickness());
    }
}


void MainWindow::on_load_gap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Gap: ","mm: ", 0, 0, 50.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_recipe.setGap(doubVal);
        // update dashboard
        ui->gap_recipe->setText(m_recipe.getGap());
        // update 3 axis tab
        ui->gap_recipe->setText(m_recipe.getGap());
    }
}


void MainWindow::on_load_overlap_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Overlap: ","mm: ", 0, 0, 5.00, 2, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_recipe.setOverlap(doubVal);
        // update dashboard
        ui->overlap_recipe->setText(m_recipe.getOverlap());
        // update 3 axis tab
        ui->input_overlap_dup->setText(m_recipe.getOverlap());
    }
}


void MainWindow::on_loadSpeedButton_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getDouble(this, "Speed: ","mm: " + m_stageCTL.getXMaxSpeedQStr(), 0, 0, m_stageCTL.XMaxSpeed(), 0, &ok,Qt::WindowFlags(), 1);
    if (ok) {
        m_recipe.setSpeed(doubVal);
        // update dashboard
        ui->speed_recipe->setText(m_recipe.getSpeed());
        // update 3 axis tab
        ui->input_speed_dup->setText(m_recipe.getSpeed());
    }
}


void MainWindow::on_load_cycles_clicked()
{
    bool ok;
    double doubVal = QInputDialog::getInt(this, "Number of Cycles","cycles: ", 0, 0, 200, 0, &ok);
    if (ok) {
        m_recipe.setCycles(doubVal);
        // update dashboard
        ui->cycles_recipe->setText(m_recipe.getCycles());
        // update 3 axis tab
        ui->input_cycles_dup->setText(m_recipe.getCycles());
    }
}













