#ifndef OPERATORTAB_H
#define OPERATORTAB_H

#include <QObject>
#include "lighttower.h"
#include "configuration.h"

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class PlasmaController;
class MainWindow;
class Configuration;

class OperatorTab : public QObject {

public:
    OperatorTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent = nullptr);
    ~OperatorTab();

    typedef enum {
        MFC6_STACKED_WIDGET_PAGE,
        MFC5_STACKED_WIDGET_PAGE,
        MFC4_STACKED_WIDGET_PAGE
    } MFC_WIDGETS;

    void testfunction();

public slots:
    // stage status to UI
    void stageStatusUpdate(const QString status1, const QString status2);
    void pinsStateChanged(const bool state);
    void vacStateChanged(const bool state);
    void axisStatusToUI();
    // home state machine
    void HSM_Startup();
    void HSM_Done();
    // init state machine
    void ISM_Startup();
    void ISM_Done();
    // collision state machine
    void CSM_StatusUpdate(const QString status, const QString next);
    // scan state machine
    void SSM_Started();
    void SSM_Done();
    void SSM_StatusUpdate(const QString status, const QString next);
    // process status
    void setUINumberOfMFCs(const int numMFCs);
    void displayAbortMessage(const QString message);
    void forwardWattsChanged(int watts);
    void reflectedWattsChanged(int watts);
    void MBactualPositionChanged(const double actualPosition);
    void headTemperatureChanged();
    void plasmaStateChanged(const bool plasmaActive);
    void thicknessChanged();
    void gapChanged();
    void updateRecipeFlow(const int mfcNumber, const double recipeFlow);
    void actualFlowChanged(const int mfcNumber, const double actualFlow);
    void lightTowerStateChanged(const LightTower::LightState state);
    void doorStateChanged(bool state);
    void mfcFlowLinesOn(const int mfcNum, const bool on);

private slots:
    void btnChuckVacOnOff_clicked(bool checked);
    void btnOPPinsUpDown_clicked(bool checked);
    void btnOPStartPlasma_clicked(bool checked);
    void btnOPAbort_clicked();
    void btnOPLoad_clicked(bool checked);
    void btnOPInit_clicked();
    void comboBoxOPRecipe_currentTextChanged(const QString &arg1);
    void comboBoxOPLogin_currentTextChanged(const QString &arg1);

private:
    void populateRecipeComboBox();
    void connectMFCFlowBars();
    void lightTowerSetGreen();
    void lightTowerSetYellow();
    void lightTowerSetRed();
    void lightTowerSetInactive();
    //void mfcFlowLinesOn(int mfcNum, bool on);

    Ui::MainWindow* m_pUI;
    MainWindow* m_pMWndw;

    PlasmaController &m_controller;
    bool m_plasmaActive;

    Configuration m_config;
};

#endif // OPERATORTAB_H
