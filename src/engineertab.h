#ifndef ENGINEERTAB_H
#define ENGINEERTAB_H

#include <QObject>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class PlasmaController;
class MainWindow;

class EngineerTab : public QObject {

public:
    EngineerTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent = nullptr);
    ~EngineerTab();

public slots:
    // Recipe parameters changed
    void on_RecipeParams_itemChanged(QTableWidgetItem *item);
    // Matchbox control start point changed
    void on_MBCtrl_itemChanged(QTableWidgetItem *item);
    // Stage parameters changed
    void on_StageParams_itemChanged(QTableWidgetItem *item);
    // Start Scan clicked
    void on_StartScan_clicked();
    // Stop Scan clicked
    void on_StopScan_clicked();
    // Auto Match Box Control toggled
    void on_AutoMBCtrl_toggled(bool checked);


private slots:
    void btnChuckVacOnOff_clicked(bool checked);
    void btnENPinsUpDown_clicked(bool checked);
    void btnENStartPlasma_clicked(bool checked);
    void btnENAbort_clicked();
    void btnENHome_clicked(bool checked);
    void btnENInit_clicked();
    void comboBoxENRecipe_currentTextChanged(const QString &arg1);
    void comboBoxENLogin_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow* m_pUI;
    MainWindow* m_pMWndw;

    PlasmaController &m_controller;
};

#endif // ENGINEERTAB_H
