#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "include/console.h"
#include "include/mainloop.h"
#include "include/commandfilereader.h"
#include "include/settingsdialog.h"
#include "include/recipe.h"
#include "include/logger.h"
#include "include/plasmacontroller/plasmarecipe.h"
#include "include/plasmacontroller/plasmacontroller.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <memory>
#include <QInputDialog>
#include <QLineEdit>


namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(MainLoop& loop, Logger& logger, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Serial Port
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

    void shutDownProgram();

    void updateRecipeProgressBar(const int& mfcNumber, const double& flow);

    void RFRecipeButton_clicked();
    void TunerRecipeButton_clicked();
    void AutoTuneCheckbox_stateChanged(int value);
    void openRecipe();
    void saveRecipe();
    void openCascadeRecipe();

    void addRecipeToCascadeRecipe();
    void removeRecipeFromCascadeList();
    void saveAsCascadeRecipeListToFile();

    void connectConsole();
private:
    // Action Button methods
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    // Serial Port methods

    // GUI signal Slot connections
    void connectMFCFlowBars();
    void connectMFCRecipeButton(QPushButton* button, const int &mfcNumber);
    // Connection for Recipes buttons
    void connectRecipeButtons();

    // Connection for Cascade Recipes buttons
    void connectCascadeRecipeButtons();

    void consoleSetup();
public slots:
    void openRecipeWindowMFC();
public:
    // Reference to the MainLoop object
    MainLoop& mainLoop;
    // Reference to the logger
    Logger& log;
    Ui::MainWindow* ui = nullptr;
    QLabel* status = nullptr;
    SettingsDialog* settings = nullptr;
    //Recipe recipe;
    PlasmaController CTL;
    PlasmaRecipe plasmaRecipe;
    QSerialPort* serial = nullptr;
    CommandFileReader commandFileReader;
    Console* console = nullptr;
};
#endif // MAINWINDOW_H
