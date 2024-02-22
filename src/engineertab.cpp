#include "engineertab.h"
#include "ui_mainwindow.h"
#include "plasmacontroller.h"
#include "UtilitiesAndConstants.h"
#include "qdir.h"
#include <QApplication>
#include "mainwindow.h"

EngineerTab::EngineerTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent) :
    QObject(parent),
    m_pUI(ui),
    m_pMWndw(mWndw),
    m_controller(controller)
{

}

EngineerTab::~EngineerTab()
{
    // do not delete here, ui object is cleaned up in MainWindow
    m_pUI = nullptr;
    m_pMWndw = nullptr;
}

void EngineerTab::on_RecipeParams_itemChanged(QTableWidgetItem *item)
{
    // extract item information
    int row = item->row();
    int column = item->column();
    QString text = item->text();

    // convert new text to integer
    bool okText;
    int newItemValue = text.toInt(&okText);

    if (!okText) {
        // Non-integer text presented to table cell, alert user that form was not accepted
    }

    switch (column) {
    case 0:
        // He changed
        break;
    case 1:
        // He/H2 changed
        break;
    case 2:
        // N2 changed
        break;
    case 3:
        // O2 changed
        break;
    case 4:
        // Power changed
        break;
    }
}

void EngineerTab::on_MBCtrl_itemChanged(QTableWidgetItem *item)
{
    // extract item text
    QString text = item->text();

    // convert new text to integer
    // convert new text to integer
    bool okText;
    int newStartPoint = text.toInt(&okText);

    if (!okText) {
        // Non-integer text presented to table cell, alert user that form was not accepted
    }

    // Matchbox start point changed

}


void EngineerTab::on_StageParams_itemChanged(QTableWidgetItem *item)
{
    // extract item information
    int row = item->row();
    int column = item->column();
    QString text = item->text();

    // convert new text to integer
    bool okText;
    int newItemValue = text.toInt(&okText);
    bool autoStartScan;

    if (!okText) {
        // Non-integer text presented to table cell, check for Y/N
        if (text == "Y") {
            autoStartScan = true;
        } else if (text == "N"){
            autoStartScan = false;
        } else {
            // Non-integer and Non-boolean text presented to table cell, alert user
        }
    }

    switch (column) {
    case 0:
        // Gap changed
        break;
    case 1:
        // Overlap changed
        break;
    case 2:
        // Scan Speed changed
        break;
    case 3:
        // Cycles changed
        break;
    case 4:
        // Auto Start Scan changed
        break;
    }

}

void EngineerTab::on_StartScan_clicked()
{
    // Start Scan

}

void EngineerTab::on_StopScan_clicked()
{
    // Stop Scan

}

void EngineerTab::on_AutoMBCtrl_toggled(bool checked)
{
    if (checked) {
        // Set Matchbox Control to auto
    } else {
        // Disable auto Matchbox Control
    }

}

