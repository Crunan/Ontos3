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
#include "include/axescontroller/axescontroller.h"
#include <chrono>

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <memory>
#include <QInputDialog>
#include <QLineEdit>
#include "diameter.h"

using namespace std::literals;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(MainLoop *loop, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void MSM_TransitionPolling();
    void MSM_TransitionStartup();
    void MSM_TransitionShutdown();
    void MSM_TransitionIdle();

public slots:
    void openRecipeWindowMFC();
    void stageStatusUpdate(QString statusNow, QString statusNext);
    void stageResponseUpdate(QString status);
    void runMainStateMachine();

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
    void setHomeButtonText(QString text);

    void about();
    void shutDownProgram();

    void updateRecipeProgressBar(const int& mfcNumber, const double& flow);

    void RFRecipeButton_clicked();
    void loadMBRecipeButton_clicked();
    void AutoTuneCheckbox_stateChanged(int value);
    void openRecipe();
    void saveRecipe();
    void openCascadeRecipe();

    void addRecipeToCascadeRecipe();
    void removeRecipeFromCascadeList();
    void saveAsCascadeRecipeListToFile();

    //void initButtonClicked();

    //void on_Joystick_button_clicked();
    void on_init_button_clicked();
    void on_init_button_dup_clicked();
    void on_Home_button_clicked();
    void on_Home_button_dup_clicked();
    void on_Stagepins_button_dup_toggled(bool checked);

    void on_n2_purge_button_dup_toggled(bool checked);

    void on_stageDisconnectButton_clicked();

    void on_diameter_button_dup_clicked();

    void on_Joystick_button_dup_toggled(bool checked);

    void on_load_thick_clicked();

    void on_load_gap_clicked();

    void on_load_overlap_clicked();

    void on_loadSpeedButton_clicked();

    void on_load_cycles_clicked();

    void on_load_autoscan_clicked();


    void on_wafer_diameter_dup_currentIndexChanged(int index);

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

    void RunStartup();
    void RunPolling();
    void GetExeCfg();
    void loadConfigGUI(QStringList value);
    void setupMainStateMachine();
    void UpdateStatus();
    void toggleN2PurgeOn();
    void toggleN2PurgeOff();
    void toggleJoystickOn();
    void toggleJoystickOff();

    void AxisStatusToUI();

    MainLoop *m_pMainLoop;
    //Logger& m_log;
    Ui::MainWindow* ui;
    QLabel* m_pStatus;
    SettingsDialog* m_pSettings;
    PlasmaController m_mainCTL;
    //GRBLController stageCTL;
    AxesController m_stageCTL;
    CommandFileReader m_commandFileReader;
    Recipe m_recipe;
    PlasmaRecipe m_plasmaRecipe; // TODO: replace with m_recipe object
    Console* m_pMainCTLConsole;
    Console* m_pStageCTLConsole;
    StageWidget* m_pStageWidget;
    Configuration m_config;

    // main state machine
    QStateMachine m_mainStateMachine;
    QState *m_pMainStartupState;
    QState *m_pMainIdleState;
    QState *m_pMainPollingState;
    QState *m_pMainShutdownState;

    Diameter m_waferDiameter;
};
#endif // MAINWINDOW_H
