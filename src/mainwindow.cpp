#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MainLoop& loop, Logger& logger, QWidget *parent) :
    QMainWindow(parent),
    mainLoop(loop),
    log(logger),
    ui(new Ui::MainWindow),
    status(new QLabel),
    settings(new SettingsDialog),
    //recipe(),
    CTL(),
    plasmaRecipe(&CTL),
    serial(nullptr),
    commandFileReader()
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE");


    // Make signal/slot connections here
    connectMFCRecipeButtons();
    connectMFCFlowBars();
    initActionsConnections();
}
MainWindow::~MainWindow() {
    delete ui;
    delete settings;
}

void MainWindow::connectSerialPort()
{
    // Get the current settings from the SettingsDialog
    SettingsDialog::Settings portSettings = settings->settings();

    // Call the openSerialPort function of CTL.serial and pass the settings
    CTL.serial.openSerialPort(portSettings);

    // Set serial CTL object for the mainwindow to use.
    this->serial = CTL.getSerialPortManager();
}


void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < CTL.mfcs.size(); ++i) {
        connect(CTL.mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateFlowBar);
    }
}

void MainWindow::connectMFCRecipeButtons()
{
    connectMFCRecipeButton(ui->pushButton, 1);
    connectMFCRecipeButton(ui->pushButton_2, 2);
    connectMFCRecipeButton(ui->pushButton_3, 3);
    connectMFCRecipeButton(ui->pushButton_4, 4);
}

void MainWindow::connectMFCRecipeButton(QPushButton* button, const int& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}

void MainWindow::openRecipeWindowMFC()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "MFC Setpoint", "Please enter a setpoint for the MFC:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        if (!CTL.mfcs.isEmpty()) {
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            if (button) {

                // Retrieve the MFC number from the button's property
                int mfcNumber = button->property("MFCNumber").toInt();  // Retrieve the MFC index from the button's property
                MFC* mfc = CTL.findMFCByNumber(mfcNumber);
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

void MainWindow::updateFlowBar(const int& mfcNumber, const double& flow)
{
    if (mfcNumber == 1) {
        ui->flowBar->setValue(flow);
    } else if (mfcNumber == 2) {
        ui->flowBar_2->setValue(flow);
    } else if (mfcNumber == 3) {
        ui->flowBar_3->setValue(flow);
    } else if (mfcNumber == 4) {
        ui->flowBar_4->setValue(flow);
    }

}

void MainWindow::initActionsConnections() {
    connect(ui->actionDisconnect, &QAction::triggered, &CTL.serial, &SerialPortManager::closeSerialPort);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::connectSerialPort);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Ontos 3 Interface"),
                   tr("The <b>Ontos3 Interface</b> is the latest"
                      "modern GUI for Plasma applications."));
}

void MainWindow::shutDownProgram() {
    if (serial) {
        serial->closeSerialPort();
    }
    Logger::clean();
    MainWindow::close();
}

void MainWindow::handleSerialPortError() {
    // Display the error to the user and log
    QMessageBox::critical(this, tr("Serial Communication Error"), serial->getError());
    log.logCritical(serial->getError());
}

//void MainWindow::createRecipe() {
//    // Delete the previous recipe if one exists
//    delete recipe;

//    // Create a new recipe object
//    recipe = new Recipe();
//    // Initialize the recipe object or perform other operations
//}
//Recipe* MainWindow::getRecipe() const{
//    return recipe;
//}



void MainWindow::RFRecipeButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "RF Setpoint", "Please enter a setpoint for RF Power:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        int recipe = recipeStr.toInt();
        CTL.pwr.setRecipeWatts(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::TunerRecipeButton_clicked()
{
    bool ok;
    QString recipeStr = QInputDialog::getText(nullptr, "Tuner Setpoint", "Please enter a setpoint for MB Tuner:", QLineEdit::Normal, "", &ok);

    if (ok && !recipeStr.isEmpty()) {
        // User entered a string and clicked OK
        double recipe = recipeStr.toDouble();
        CTL.tuner.setRecipePosition(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::AutoTuneCheckbox_stateChanged(int value)
{
    CTL.tuner.setAutoTune(value);
}

