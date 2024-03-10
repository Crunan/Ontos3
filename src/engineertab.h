#ifndef ENGINEERTAB_H
#define ENGINEERTAB_H

#include <QObject>
#include <QTableWidgetItem>
#include "configuration.h"

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class PlasmaController;
class MainWindow;
class QInputDialog;

class EngineerTab : public QObject {

public:
    EngineerTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent = nullptr);
    ~EngineerTab();

public slots:
    void on_RecipeParams_itemChanged(QTableWidgetItem *item);
    void on_MBCtrl_itemChanged(QTableWidgetItem *item);
    void on_StageParams_itemChanged(QTableWidgetItem *item);
    void on_StartScan_clicked();
    void on_StopScan_clicked();
    void on_AutoMBCtrl_toggled(bool checked);
    void Recipe_MFCSetpointAccepted();
    void updateRecipeFlow(const int mfcNumber, const double recipeFlow);
    void recipeWattsChanged(int watts);

    void stageParams_cellClicked(int row, int column);
    void recipeParams_cellClicked(int row, int column);
    void recipeParams_itemClicked(QTableWidgetItem *item);

private slots:
    void btnChuckVacOnOff_clicked(bool checked);
    void btnENPinsUpDown_clicked(bool checked);
    void btnENStartPlasma_clicked(bool checked);
    void btnENAbort_clicked();
    void btnENHome_clicked(bool checked);
    void btnENInit_clicked();
    void comboBoxENRecipe_currentTextChanged(const QString &arg1);
    void comboBoxENLogin_currentTextChanged(const QString &arg1);
    void tablewidgetRecipeParams_cellEntered(int row, int column);
    void tablewidgetRecipeParams_cellChanged(int row, int column);

private:
    void MFCRecipeValueChange(int mfcNumber);
    void connectMFCRecipeFlow();

    void setMFCLabels();

    // updates MFC UI components
    void updateActualFlow_4MFC(const int mfcNumber, const double actualFlow);
    void updateActualFlow_6MFC(const int mfcNumber, const double actualFlow);

    Ui::MainWindow* m_pUI;
    MainWindow* m_pMWndw;

    PlasmaController &m_controller;

    QInputDialog *m_pRecipeInputDialog;

    int m_numMFCs;
    Configuration m_config;
};

#endif // ENGINEERTAB_H
