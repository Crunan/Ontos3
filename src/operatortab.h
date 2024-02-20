#ifndef OPERATORTAB_H
#define OPERATORTAB_H

#include <QObject>
#include "lighttower.h"

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class PlasmaController;
class MainWindow;

class OperatorTab : public QObject {

public:
    OperatorTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent = nullptr);
    ~OperatorTab();

public slots:
    // stage status to UI
    void stageStatusUpdate(QString status1, QString status2);
    void pinsStateChanged(bool state);
    void vacStateChanged(bool state);
    void axisStatusToUI();
    // home state machine
    void HSM_Startup();
    void HSM_Done();
    // init state machine
    void ISM_Startup();
    void ISM_Done();
    // collision state machine
    void CSM_StatusUpdate(QString status, QString next);
    // scan state machine
    void SSM_Started();
    void SSM_Done();
    void SSM_StatusUpdate(QString status, QString next);
    // process status
    void forwardWattsChanged();
    void MBactualPositionChanged(const double actualPosition);
    void headTemperatureChanged();
    void plasmaStateChanged(bool plasmaActive);
    void thicknessChanged();
    void gapChanged();
    void updateRecipeFlow(const int mfcNumber, const double recipeFlow);
    void actualFlowChanged(const int mfcNumber, const double actualFlow);
    void lightTowerStateChanged(LightTower::LightState state);

    void mfcFlowLinesOn(int mfcNum, bool on);

private slots:
    void btnChuckVacOnOff_clicked(bool checked);
    void btnOPPinsUpDown_clicked(bool checked);
    void btnOPStartPlasma_clicked(bool checked);
    void btnOPAbort_clicked();
    void btnOPHome_clicked(bool checked);
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
};

#endif // OPERATORTAB_H
