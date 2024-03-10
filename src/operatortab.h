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

    void pinsStateChanged(const bool state);
    void vacStateChanged(const bool state);
    void axisStatusToUI();
    void HSM_Done();
    void ISM_Done();
    void SSM_Started();
    void SSM_Done();
    void setUINumberOfMFCs(const int numMFCs);
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

private slots:
    void comboBoxOPLogin_currentTextChanged(const QString &arg1);

private:
    void connectMFCFlowBars();
    void lightTowerSetGreen();
    void lightTowerSetYellow();
    void lightTowerSetRed();
    void lightTowerSetInactive();

    // updates MFC UI components
    void updateRecipeFlow_4MFC(const int mfcNumber, const double recipeFlow);
    void updateRecipeFlow_6MFC(const int mfcNumber, const double recipeFlow);
    void updateActualFlow_4MFC(const int mfcNumber, const double actualFlow);
    void updateActualFlow_6MFC(const int mfcNumber, const double actualFlow);
    void mfcFlowLinesOn_4MFC(const int mfcNum, const bool on);
    void mfcFlowLinesOn_6MFC(const int mfcNum, const bool on);

    void setMFCLabels();

    Ui::MainWindow* m_pUI;
    MainWindow* m_pMWndw;

    PlasmaController &m_controller;
    bool m_plasmaActive;

    Configuration m_config;
    int m_numMFCs;
    int m_currentReflectedWatts;
};

#endif // OPERATORTAB_H
