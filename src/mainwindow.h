#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grblcontroller.h"
#include "mainloop.h"
#include "settingsdialog.h"
#include "logger.h"
#include "plasmacontroller.h"
#include "axescontroller.h"
#include <chrono>


#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <memory>
#include <QInputDialog>
#include <QLineEdit>
#include "passworddialog.h"
#include <QSettings>
#include <QFileDialog>
#include "gamepadcontroller.h"


class OperatorTab;
class EngineerTab;

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

    //gets called right before the main window is about to show
    void showEvent(QShowEvent *) override;

    typedef enum {
        OPERATOR_TAB,
        ENGINEER_TAB
    } tabs;

    void populateRecipeComboBox();

    // access control
    void engineerModeSelected() {     // clear the password
        m_passDialog.clearPassword();
        m_passDialog.show();
        m_passDialog.setFocusLineEdit();
    }

    void setUIOperatorMode();
    void setUIEngineerMode();

signals:
    // main state machine state transiton signals
    void MSM_TransitionPolling();
    void MSM_TransitionStartup();
    void MSM_TransitionShutdown();
    void MSM_TransitionIdle();

public slots:
    void setInitialUIState();
    void openRecipeWindowMFC();
    void showAbortMessageBox(QString message, bool shutdown);
    void batchIDLoggingIsActive();

    // Serial Port
    void openMainPort();
    void closeMainPort();
    void readTimeoutError(QString lastCommand);

    // state machine
    void runMainStateMachine();

    // recipe
    void AutoTuneCheckbox_stateChanged(int value);
    void openRecipe();
    void saveRecipe();
    void installRecipe(QString sRecipeFileAndPath);
    void setRecipeMBtuner(double MBtunerSP);
    void thicknessChanged();
    void gapChanged();
    void overlapChanged();
    void speedChanged();
    void autoScanChanged();
    void xLimitsChanged();
    void yLimitsChanged();
    void cyclesChanged();
    void loadCascadeRecipe();
    void recipeWattsChanged();
    void autoTuneChanged(bool autoTune);

    // callbacks
    void joystickStateChanged(bool state);
    void userEnteredPassword();

    // recipe set value slots
    void Recipe_RFSetpointAccepted();
    void Recipe_MBSetpointAccepted();
    void Recipe_MFCSetpointAccepted();
    void Recipe_ThicknessAccepted();
    void Recipe_X1Accepted();
    void Recipe_X2Accepted();
    void Recipe_Y1Accepted();
    void Recipe_Y2Accepted();
    void Recipe_CyclesAccepted();
    void Recipe_SpeedAccepted();
    void Recipe_OverlapAccepted();
    void Recipe_GapAccepted();
    void Recipe_BatchIDAccepted();
    void SaveRecipeFileSelected(const QString &file);
    void OpenRecipeFileSelected(const QString &file);
    void OpenRecipeRejected();
    void SaveRecipeFileRejected();

    // program control actions
    //void displayAbortMessage(const QString message);
    void stageStatusUpdate(const QString status1, const QString status2);
    void CSM_StatusUpdate(const QString status, const QString next);
    void SSM_Started();
    void SSM_StatusUpdate(const QString status, const QString next);
    void SSM_Done();
    void ISM_Startup();
    void ISM_Done();
    void HSM_Startup();
    void HSM_Done();
    void pinsStateChanged(const bool state);
    void vacStateChanged(const bool state);
    void plasmaStateChanged(const bool plasmaActive);
    void displayAbortMessage(QString smsg);

private slots:
    // controls handlers
    void on_load_thick_clicked();
    void on_load_gap_clicked();
    void on_load_overlap_clicked();
    void on_loadSpeedButton_clicked();
    void on_load_cycles_clicked();
    void on_loadRecipeButton_clicked();
    void on_loadRFButton_clicked();
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
    void on_x1_set_clicked();
    void on_x2_set_clicked();
    void on_y1_set_clicked();
    void on_y2_set_clicked();
    void on_actionAbout_triggered();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSettings_triggered();
    void on_MB_Right_Button_clicked();
    void on_MB_Left_Button_clicked();
    void on_actionSet_Default_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionDetailed_Log_toggled(bool arg1);
    void on_actionTest_Z_toggled(bool arg1);
    void on_batchID_checkBox_clicked(bool checked);
    void on_collision_system_checkbox_clicked(bool checked);
    void on_batchIDButton_clicked();
    void on_refresh_cascade_recipe_button_clicked();
    void on_LEDIntensitySpinBox_valueChanged(double arg1);
    void on_mainTabWidget_currentChanged(int index);
    void on_pushButton_clicked(bool checked);


    void btnChuckVacOnOff_clicked(bool checked);
    void btnPinsUpDown_clicked(bool checked);
    void btnStartPlasma_clicked(bool checked);
    // void btnAbort_clicked();
    void btnLoad_clicked(bool checked);
    void btnInit_clicked();
    void comboBoxRecipe_currentTextChanged(const QString &arg1);
    void btnAcknowledged_clicked();

private:

    // Action Button methods
    void showStatusMessage(const QString &message);
    // hit x in top right
    void closeEvent(QCloseEvent *event);
    // serial port signals sent from serialInterface
    void serialDisconnected();
    void serialConnected();
    void disableControlButtons();
    void connectMFCRecipeButton(QPushButton* button, const int &mfcNumber);
    // Connection for Recipes buttons
    void connectRecipeButtons();
    // Connection for Cascade Recipes buttons
    void connectCascadeRecipeButtons();
    // Function for loading recipes into Qlist
    void populateRecipeListWidgetFromDirectory(QListWidget* listWidget);
    void consoleMainCTLSetup();
    void batchIDEnabled();
    // persistent settings
    void readSettings();
    // determine if there is a 3 axis board attached
    void has3Axis();

    // helpers
    void RunStartup();
    void RunStatusPolling();
    void GetExeCfg();
    void setupMainStateMachine();
    void UpdateStatus();
    void showStageControls(bool show);

    void AxisStatusToUI();
    void RecipeToUI();
    void setMFCLabels();

    // helper function that connects the individual
    // tabs UI components slots to their respective
    // signals
    void connectOperatorTabSlots();
    void connectEngineerTabSlots();

    OperatorTab *m_pOperatortab;
    EngineerTab *m_pEngineertab;

    MainLoop *m_pMainLoop;
    Ui::MainWindow* ui;
    QLabel* m_pStatus;
    PasswordDialog m_passDialog;
    SettingsDialog* m_pSettings;
    PlasmaController m_mainCTL;
    QSettings m_persistentSettings;

    // recipe non modal dialogs
    QFileDialog *m_pRecipeFileDialog;
    QInputDialog *m_pRecipeInputDialog;

    GamepadController m_gamepadController;

    bool m_has3AxisBoard;

    Configuration m_config;
    bool m_engineeringMode;

    // main state machine
    QStateMachine m_mainStateMachine;
    QState *m_pMainStartupState;
    QState *m_pMainIdleState;
    QState *m_pMainPollingState;
    QState *m_pMainShutdownState;

    bool m_plasmaActive;
};
#endif // MAINWINDOW_H
