#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "include/mainloop.h"
#include "include/commandfilereader.h"
#include "include/settingsdialog.h"
#include "include/recipe.h"
#include "include/logger.h"
#include "include/serialportmanager.h"
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

private:

    // Action Button methods
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    // Serial Port methods

    // GUI signal Slot connections
    void connectMFCRecipeButtons();
    void connectMFCFlowBars();
    void connectMFCRecipeButton(QPushButton* button, const int &mfcNumber);



private slots:
    void updateFlowBar(const int& mfcNumber, const double& flow);
    void connectSerialPort();

    void RFRecipeButton_clicked();

    void TunerRecipeButton_clicked();

    void AutoTuneCheckbox_stateChanged(int value);

    void loadRecipeButton_clicked();

public slots:
    void about();
    void shutDownProgram();
    void openRecipeWindowMFC();    
    void handleSerialPortError();
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
    SerialPortManager* serial = nullptr;
    CommandFileReader commandFileReader;
};
#endif // MAINWINDOW_H
