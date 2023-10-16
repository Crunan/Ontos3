#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "console.h"
#include "grblcontroller.h"
#include "mainloop.h"
#include "commandfilereader.h"
#include "settingsdialog.h"
#include "logger.h"
#include "plasmacontroller.h"
#include "stagewidget.h"
#include "axescontroller.h"
#include <chrono>

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <memory>
#include <QInputDialog>
#include <QLineEdit>

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
    // main state machine state transiton signals
    void MSM_TransitionPolling();
    void MSM_TransitionStartup();
    void MSM_TransitionShutdown();
    void MSM_TransitionIdle();

public slots:
    void openRecipeWindowMFC();
    void stageStatusUpdate(QString statusNow, QString statusNext);

    // Main CTL Serial Port
    void openMainPort();
    void closeMainPort();
    void writeMainPort(const QByteArray &data);
    QString readMainPort();
    void handleMainSerialError(QSerialPort::SerialPortError error);

    // state machine ui updating slots
    void homeStateMachineStartup();
    void homeStateMachineDone();
    void initStateMachineStartup();
    void initStateMachineDone();
    void twoSpotStateMachineStartup();
    void twoSpotStateMachineDone();
    void runMainStateMachine();

    // real time data updates
    void recipeWattsChanged();
    void autoTuneChanged(bool autoTune);
    void forwardWattsChanged();
    void reflectedWattsChanged();
    void MBactualPositionChanged(const double actualPosition);
    void headTemperatureChanged();

    // mfc
    void connectMFCFlowBars();
    void updateRecipeFlow(const int mfcNumber, const double recipeflow);
    void actualFlowChanged(const int mfcNumber, const double actualFlow);

    void about();
    void shutDownProgram();

    void AutoTuneCheckbox_stateChanged(int value);
    void openRecipe();
    void saveRecipe();
    void openCascadeRecipe();

    void setRecipeMBtuner(double MBtunerSP);
  //  void plasmaHeadTemp(double temp);
    void thicknessChanged();
    void gapChanged();
    void overlapChanged();
    void speedChanged();
    void autoScanChanged();
    void xLimitsChanged();
    void yLimitsChanged();
    void cyclesChanged();

    void pinsStateChanged(bool state);
    void joystickStateChanged(bool state);
    void n2StateChanged(bool state);
    void vacStateChanged(bool state);
    void CSM_StatusUpdate(QString status, QString next);
    void SSM_Started();
    void SSM_Done();
    void SSM_StatusUpdate(QString status, QString next);
    void recipeExecutionStateChanged(bool state);
    void scanBoxChanged();

private slots:
    // button handlers
    void on_init_button_clicked();
    void on_init_button_dup_clicked();
    void on_Stagepins_button_dup_toggled(bool checked);
    void on_n2_purge_button_dup_toggled(bool checked);
    void on_diameter_button_dup_clicked();
    void on_load_thick_clicked();
    void on_load_gap_clicked();
    void on_load_overlap_clicked();
    void on_loadSpeedButton_clicked();
    void on_load_cycles_clicked();
    void on_wafer_diameter_dup_currentIndexChanged(int index);
    void on_wafer_diameter_currentIndexChanged(int index);
    void on_vac_button_dup_toggled(bool checked);
    void on_vac_button_toggled(bool checked);
    void on_n2_purge_button_toggled(bool checked);
    void on_Joystick_button_dup_toggled(bool checked);
    void on_twospot_button_dup_toggled(bool checked);
    void on_Home_button_toggled(bool checked);
    void on_twospot_button_toggled(bool checked);
    void on_Home_button_dup_toggled(bool checked);
    void on_loadRecipeButton_clicked();
    void on_loadRFButton_clicked();
    void on_scan_button_dup_toggled(bool checked);
    void on_scan_button_toggled(bool checked);
    void on_plsmaBtn_toggled(bool checked);
    void on_plsmaBtn_dup_toggled(bool checked);
    void on_loadAutoTuneButton_clicked();
    void on_loadMBButton_clicked();
    void on_load_autoscan_clicked();
    void on_save_recipe_button_clicked();
    void on_saveAsCascadeRecipeButton_clicked();
    void on_loadCascadeRecipeButton_clicked();
    void on_addCascadeRecipeButton_clicked();
    void on_removeCascadeRecipeButton_clicked();
    void on_clear_cascade_recipe_button_clicked();
    void on_Joystick_button_toggled(bool checked);
    void on_Stagepins_button_toggled(bool checked);   
    void on_diameter_button_clicked();

private:
    // Action Button methods
    void serialButtonPreConnectState();
    void showStatusMessage(const QString &message);
    // Serial Port methods

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

    void AxisStatusToUI();
    void RecipeToUI();

    MainLoop *m_pMainLoop;
    Ui::MainWindow* ui;
    QLabel* m_pStatus;
    SettingsDialog* m_pSettings;
    PlasmaController m_mainCTL;

    CommandFileReader m_commandFileReader;

    Console* m_pMainCTLConsole;
    //Console* m_pStageCTLConsole;
    StageWidget* m_pStageWidget;
    Configuration m_config;

    // main state machine
    QStateMachine m_mainStateMachine;
    QState *m_pMainStartupState;
    QState *m_pMainIdleState;
    QState *m_pMainPollingState;
    QState *m_pMainShutdownState;
};
#endif // MAINWINDOW_H
