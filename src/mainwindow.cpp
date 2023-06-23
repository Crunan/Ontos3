#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MainLoop& loop, Logger& logger, QWidget *parent) :
    QMainWindow(parent),
    mainLoop(loop),
    log(logger),
    ui(new Ui::MainWindow),
    status(new QLabel),
    settings(new SettingsDialog),
    serial(std::make_shared<SerialComms>()),
    recipe(nullptr),
    CTL(*serial),
    commandFileReader()
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE");
    // Make signal/slot connections here
    commandFileReader.setCommandFilePath("commands/");
    commandFileReader.setCommandFileName("commands.ini");
    QMap CTLCommands = commandFileReader.readCommandsFromFile();
    CTL.setCommandMap(CTLCommands);
    initActionsConnections();
}
MainWindow::~MainWindow() {
    delete ui;
    delete settings;
    delete recipe;
}

void MainWindow::initActionsConnections() {
    connect(ui->actionConnect, &QAction::triggered, this, [this]() {
        // Call the openSerialPort slot with the settings object
        serial->openSerialPort(settings->settings());
    });
    connect(ui->actionDisconnect, &QAction::triggered, serial.get(), &SerialComms::closeSerialPort);
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
