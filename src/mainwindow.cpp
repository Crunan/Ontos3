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
    mainCTL(),
    stageCTL(),
    commandFileReader(),
    plasmaRecipe(&mainCTL),
    mainCTLConsole(),
    stageCTLConsole(),
    stageWidget(new StageWidget(this))
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
    connect(ui->stageDisconnectButton, &QPushButton::clicked, this, &MainWindow::closeStagePort);
    // Settings button
    connect(ui->mainSettingsButton, &QPushButton::clicked, settings, &SettingsDialog::show);
    connect(ui->stageSettingsButton, &QPushButton::clicked, settings, &SettingsDialog::show);

    // TODO: create stage area for custom pathing
    ui->verticalLayout_4->addWidget(stageWidget);
    stageWidget->setStageBounds(0.0, 100.0, 0.0, 50.0);

    // Make signal/slot connections here
    connectRecipeButtons();
    connectCascadeRecipeButtons();
    connectMFCFlowBars();

    // status bar
    ui->statusBar->addWidget(status);

}
MainWindow::~MainWindow() {
    delete settings;
    delete ui;
}

void MainWindow::openMainPort()
{
    const SettingsDialog::Settings p = settings->settings();
    if (mainCTL.open(*settings)) {

        //Terminal Tab setup for console commands
        consoleMainCTLSetup();        
        mainCTLConsole->setEnabled(true);
        mainCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        // Update UI buttons
        ui->mainConnectButton->setEnabled(false);
        ui->mainDisconnectButton->setEnabled(true);
        ui->mainSettingsButton->setEnabled(false);

        // Give status on connect
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), mainCTL.getPortErrorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::openStagePort()
{
    const SettingsDialog::Settings p = settings->settings();
    if (stageCTL.open(*settings)) {

        //Terminal Tab setup for console commands
        consoleStageCTLSetup();
        stageCTLConsole->setEnabled(true);
        stageCTLConsole->setLocalEchoEnabled(p.localEchoEnabled);

        // Update UI buttons
        ui->stageConnectButton->setEnabled(false);
        ui->stageDisconnectButton->setEnabled(true);
        ui->stageSettingsButton->setEnabled(false);

        // Give status on connect
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), stageCTL.getPortErrorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeMainPort()
{
    if (mainCTL.isOpen()) {
        mainCTL.close();
    }

    // Disable Console
    mainCTLConsole->setEnabled(false);

    // Default button states
    ui->mainConnectButton->setEnabled(true);
    ui->mainDisconnectButton->setEnabled(false);
    ui->mainSettingsButton->setEnabled(true);

    // Update Status bar
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::closeStagePort()
{
    if (stageCTL.isOpen()) {
        stageCTL.close();
    }

    // Disable Console
    stageCTLConsole->setEnabled(false);

    // Default button states
    ui->stageConnectButton->setEnabled(true);
    ui->stageDisconnectButton->setEnabled(false);
    ui->stageSettingsButton->setEnabled(true);

    // Update Status bar
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::writeMainPort(const QByteArray &data)
{
    if (!mainCTL.sendCommand(data))
        qDebug() << "Command not sent to main CTL";
}

void MainWindow::writeStagePort(const QByteArray &data)
{
    if (!stageCTL.sendCommand(data))
        qDebug() << "Command not sent to stage CTL";
}

QString MainWindow::readMainPort()
{
    return mainCTL.readData();
}

QString MainWindow::readStagePort()
{
    return stageCTL.readData();

    // Update console without side effect
    //stageCTLConsole->putData(data);

}
void MainWindow::handleMainSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), mainCTL.getPortErrorString());
        log.logCritical(mainCTL.getPortErrorString());
        closeMainPort();
    }
}

void MainWindow::handleStageSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), stageCTL.getPortErrorString());
        log.logCritical(stageCTL.getPortErrorString());
        closeStagePort();
    }
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Ontos 3 Interface"),
                   tr("The <b>Ontos3 Interface</b> is the latest"
                      "modern GUI for Plasma applications."));
}

void MainWindow::shutDownProgram() {
    if (mainCTL.isOpen()) {
        mainCTL.close();
    }
    if (stageCTL.isOpen()) {
        stageCTL.close();
    }
    Logger::clean();
    MainWindow::close();
}

void MainWindow::consoleMainCTLSetup()
{
    // Step 1: Create an instance of the console class
    mainCTLConsole = new Console(ui->mainTabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->mainTabWidget->addTab(mainCTLConsole, "Main CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(mainCTLConsole, &Console::getData, this, &MainWindow::writeMainPort);
}

void MainWindow::consoleStageCTLSetup()
{
    // Step 1: Create an instance of the console class
    stageCTLConsole = new Console(ui->mainTabWidget);

    // Step 2: Add the console instance to a new tab
    int tabIndex = ui->mainTabWidget->addTab(stageCTLConsole, "Stage CTL Terminal");

    // Step 3: Set the Qt theme icon for the tab
    QIcon icon = QIcon::fromTheme("utilities-system");
    ui->mainTabWidget->setTabIcon(tabIndex, icon);

    // Step 4: connect signals/slot
    connect(stageCTLConsole, &Console::getData, this, &MainWindow::writeStagePort);
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

void MainWindow::connectMFCFlowBars()
{
    // This will connect the flowchanged signal along with its passed params
    // to the GUI updateFlowbars function.
    for (int i = 0; i < mainCTL.mfcs.size(); ++i) {
        connect(mainCTL.mfcs[i], &MFC::recipeFlowChanged, this, &MainWindow::updateRecipeProgressBar);
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
        if (!mainCTL.mfcs.isEmpty()) {
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            if (button) {

                // Retrieve the MFC number from the button's property
                int mfcNumber = button->property("MFCNumber").toInt();  // Retrieve the MFC index from the button's property
                MFC* mfc = mainCTL.findMFCByNumber(mfcNumber);
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
        mainCTL.pwr.setRecipeWatts(recipe);
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
        mainCTL.tuner.setRecipePosition(recipe);
    }
    else {
        // User either clicked Cancel or did not enter any string
        // Handle accordingly
        return;
    }
}


void MainWindow::AutoTuneCheckbox_stateChanged(int value)
{
    mainCTL.tuner.setAutoTune(value);
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

