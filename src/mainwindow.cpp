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

    // Load Commands for Controller
    commandFileReader.setCommandFilePath("commands/");
    commandFileReader.setCommandFileName("commands.ini");
    QMap CTLCommands = commandFileReader.readCommandsFromFile();
    CTL.setCommandMap(CTLCommands);

    // Make signal/slot connections here
    connectMFCButtons();

    initActionsConnections();
}
MainWindow::~MainWindow() {
    delete ui;
    delete settings;
    delete recipe;
}

void MainWindow::connectMFCButtons()
{
    connectMFCButton(ui->pushButton, "MFC1");
    connectMFCButton(ui->pushButton_2, "MFC2");
    connectMFCButton(ui->pushButton_3, "MFC3");
    connectMFCButton(ui->pushButton_4, "MFC4");
}

void MainWindow::connectMFCButton(QPushButton* button, const QString& mfcNumber)
{
    button->setProperty("MFCNumber", mfcNumber);  // Store the MFC index in the button's property
    connect(button, &QPushButton::clicked, this, &MainWindow::openRecipeWindowMFC);
}

void MainWindow::openRecipeWindowMFC()
{
    bool ok;
    QString recipe = QInputDialog::getText(nullptr, "MFC Setpoint", "Please enter a setpoint for the MFC:", QLineEdit::Normal, "", &ok);

    if (ok && !recipe.isEmpty()) {
        // User entered a string and clicked OK
        if (!CTL.mfcs.isEmpty()) {
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            if (button) {

                // Retrieve the MFC number from the button's property
                QString mfcNumber = button->property("MFCNumber").toString();  // Retrieve the MFC index from the button's property
                MFC* mfc = findMFCByNumber(mfcNumber);
                if (mfc) {
                    mfc->setLoadedSetpoint(recipe);
                }
            }
        }
    } else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}

MFC* MainWindow::findMFCByNumber(const QString& mfcNumber)
{
    for (MFC* mfc : CTL.mfcs) {
        if (mfc->getMFCNumber() == mfcNumber) {
            return mfc;
        }
    }
    return nullptr;
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
