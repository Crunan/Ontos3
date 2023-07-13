#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "include/console.h"
#include "include/grblcontroller.h"
#include "include/mainloop.h"
#include "include/commandfilereader.h"
#include "include/settingsdialog.h"
#include "include/recipe.h"
#include "include/logger.h"
#include "include/plasmacontroller/plasmarecipe.h"
#include "include/plasmacontroller/plasmacontroller.h"
#include "include/stagewidget.h"

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
    // Main CTL Serial Port
    void openMainPort();
    void closeMainPort();
    void writeMainPort(const QByteArray &data);
    QString readMainPort();

    void handleMainSerialError(QSerialPort::SerialPortError error);

    // Stage CTL Serial Port
    void openStagePort();
    void closeStagePort();
    void writeStagePort(const QByteArray &data);
    QString readStagePort();

    void handleStageSerialError(QSerialPort::SerialPortError error);

    void about();
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

private:
    // Action Button methods
    void serialButtonPreConnectState();
    void showStatusMessage(const QString &message);
    // Serial Port methods

    // GUI signal Slot connections
    void connectMFCFlowBars();
    void connectMFCRecipeButton(QPushButton* button, const int &mfcNumber);
    // Connection for Recipes buttons
    void connectRecipeButtons();

    // Connection for Cascade Recipes buttons
    void connectCascadeRecipeButtons();

    void consoleMainCTLSetup();
    void consoleStageCTLSetup();
public slots:
    void openRecipeWindowMFC();
public:
    MainLoop& mainLoop;
    Logger& log;
    Ui::MainWindow* ui = nullptr;
    QLabel* status = nullptr;
    SettingsDialog* settings = nullptr;
    PlasmaController mainCTL;
    GRBLController stageCTL;
    CommandFileReader commandFileReader;
    PlasmaRecipe plasmaRecipe;
    Console* mainCTLConsole = nullptr;
    Console* stageCTLConsole = nullptr;
    StageWidget* stageWidget;
};
#endif // MAINWINDOW_H
