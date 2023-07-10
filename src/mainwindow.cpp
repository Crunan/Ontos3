#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

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
    serial(new QSerialPort(this)),
    commandFileReader(),
    console(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("ONTOS3 INTERFACE");

    //Terminal Tab setup for console commands
    consoleSetup();

    // Connect button enable/disabled
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    // Make signal/slot connections here
    connectRecipeButtons();
    connectCascadeRecipeButtons();
    connectMFCFlowBars();

    // status bar
    ui->statusBar->addWidget(status);

    initActionsConnections();

    connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(console, &Console::getData, this, &MainWindow::writeData);
}
MainWindow::~MainWindow() {
    delete settings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    const QByteArray data = serial->readAll();
    console->putData(data);

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        log.logCritical(serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}


void MainWindow::initActionsConnections() {
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
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
        closeSerialPort();
    }
    Logger::clean();
    MainWindow::close();
}

void MainWindow::consoleSetup()
{
    // Step 1: Create an instance of the console class
    console = new Console(ui->tabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->tabWidget->addTab(console, "Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    ui->tabWidget->setTabIcon(tabIndex, icon);

    // Step 4: disable until connected
    console->setEnabled(false);
}
void MainWindow::connectRecipeButtons()
{
    connect(ui->RFRecipeButton, &QPushButton::clicked, this, &MainWindow::RFRecipeButton_clicked);
    connect(ui->TunerRecipeButton, &QPushButton::clicked, this, &MainWindow::TunerRecipeButton_clicked);
    connect(ui->AutoTuneCheckBox, &QCheckBox::stateChanged, this, &MainWindow::AutoTuneCheckbox_stateChanged);
    connect(ui->loadRecipeButton, &QPushButton::clicked, this, &MainWindow::openRecipe);
    //MFC buttons
    connectMFCRecipeButton(ui->pushButton, 1);
    connectMFCRecipeButton(ui->pushButton_2, 2);
    connectMFCRecipeButton(ui->pushButton_3, 3);
    connectMFCRecipeButton(ui->pushButton_4, 4);
}

void MainWindow::connectCascadeRecipeButtons()
{
    connect(ui->addCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::addRecipeToCascadeRecipe);
    connect(ui->removeCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::removeRecipeFromCascadeList);
    connect(ui->saveAsCascadeRecipeButton, &QPushButton::clicked, this, &MainWindow::saveAsCascadeRecipeListToFile);
}

void MainWindow::connectConsole()
{
    SettingsDialog::Settings portSettings = settings->settings();

    console->setEnabled(true);
    console->setLocalEchoEnabled(portSettings.localEchoEnabled);
}

void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < CTL.mfcs.size(); ++i) {
        connect(CTL.mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeProgressBar);
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
    if (mfcNumber == 1) {
        ui->recipeProgressBar->setValue(flow);
    } else if (mfcNumber == 2) {
        ui->recipeProgressBar_2->setValue(flow);
    } else if (mfcNumber == 3) {
        ui->recipeProgressBar_3->setValue(flow);
    } else if (mfcNumber == 4) {
        ui->recipeProgressBar_4->setValue(flow);
    }
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
        plasmaRecipe.fileReader.setFilePath(filePath);
        plasmaRecipe.setRecipeFromFile();

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

            QMap<QString, QVariant> recipe = plasmaRecipe.getRecipeMap();
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
        plasmaRecipe.addRecipeToCascade(fileName);

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
        plasmaRecipe.removeRecipeFromCascade(recipeFileName);
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
            for (const QString& recipeName : plasmaRecipe.getCascadeRecipeList()) {
                out << recipeName << "\n";
            }

            file.close();
            qDebug() << "Cascade recipe list saved to file: " << filePath;
        } else {
            qDebug() << "Failed to open file for writing: " << file.errorString();
        }
    }
}

