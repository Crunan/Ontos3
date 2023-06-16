#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MainLoop& loop, Logger& logger, QWidget *parent) :
    QMainWindow(parent),
    mainLoop(loop),
    log(logger),
    ui(new Ui::MainWindow),
    status(new QLabel),
    settings(new SettingsDialog),
    serial(new SerialComms(this)),
    recipe(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE");
    // Make signal/slot connections here
    connect(serial, &SerialComms::serialPortOpened, this, &MainWindow::SerialPortConnectedSetupFunctions);
    connect(serial, &SerialComms::errorOccured, this, &MainWindow::handleSerialPortError);
    initActionsConnections();
}
MainWindow::~MainWindow() {
    delete ui;
    delete settings;
    delete serial;
    delete recipe;
}

void MainWindow::initActionsConnections() {
    connect(ui->actionConnect, &QAction::triggered, serial, &SerialComms::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, serial, &SerialComms::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::shutDownProgram);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
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
void MainWindow::SerialPortConnectedSetupFunctions() {
    adjustButtonAvailabilityOnSerialConnect();
    displayStatusSerialPortConnected();
}

void MainWindow::displayStatusSerialPortConnected() {
    status->showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                  .arg(settings.name).arg(settings.stringBaudRate).arg(settings.stringDataBits)
                                  .arg(settings.stringParity).arg(settings.stringStopBits).arg(settings.stringFlowControl));
}
void MainWindow::adjustButtonAvailabilityOnSerialConnect() {
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    ui->actionConfigure->setEnabled(false);
}
void MainWindow::createRecipe() {
    // Delete the previous recipe if one exists
    delete recipe;

    // Create a new recipe object
    recipe = new Recipe();
    // Initialize the recipe object or perform other operations
}
Recipe* MainWindow::getRecipe() const{
    return recipe;
}
