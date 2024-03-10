#include "engineertab.h"
#include "ui_mainwindow.h"
#include "plasmacontroller.h"
#include "UtilitiesAndConstants.h"
#include "qdir.h"
#include <QApplication>
#include "mainwindow.h"
#include <QInputDialog>

EngineerTab::EngineerTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent) :
    QObject(parent),
    m_pUI(ui),
    m_pMWndw(mWndw),
    m_controller(controller),
    m_pRecipeInputDialog(new QInputDialog(mWndw)),
    m_numMFCs(6),
    m_config()
{
    connect(m_pUI->tablewidgetRecipeParams, &QTableWidget::cellClicked, this, &EngineerTab::recipeParams_cellClicked);
    connect(m_pUI->tablewidgetStageParams, &QTableWidget::cellClicked, this, &EngineerTab::stageParams_cellClicked);

    // forward power
    connect(&m_controller.getPower(), &PWR::recipeWattsChanged, this, &EngineerTab::recipeWattsChanged);

    connectMFCRecipeFlow();

    setMFCLabels();
}

EngineerTab::~EngineerTab()
{
    // do not delete here, ui object is cleaned up in MainWindow
    m_pUI = nullptr;
    m_pMWndw = nullptr;
}

void EngineerTab::connectMFCRecipeFlow()
{
    for (int i = 0; i < m_controller.getMFCs().size(); ++i) {
        connect(m_controller.getMFCs()[i], &MFC::recipeFlowChanged, this, &EngineerTab::updateRecipeFlow);
        //connect(m_controller.getMFCs()[i], &MFC::updateUIActualFlow, this, &OperatorTab::actualFlowChanged);
    }
}

void EngineerTab::setMFCLabels()
{
    for (int i = 0; i < m_numMFCs; i++) {

        QTableWidgetItem *item = m_pUI->tablewidgetRecipeParams->item(i, 0); // row, column
        if (item == nullptr) {
            item = new QTableWidgetItem;
            m_pUI->tablewidgetRecipeParams->setItem(i, 0, item); // row, column, item
        }

        QString MFC_label;

        switch (i) {
        case 0:
            MFC_label = m_config.getValueForKey(CONFIG_MFC1_LABEL_KEY);
            item->setText(MFC_label);
            break;
        case 1:
            MFC_label = m_config.getValueForKey(CONFIG_MFC2_LABEL_KEY);
            item->setText(MFC_label);
            break;
        case 2:
            MFC_label = m_config.getValueForKey(CONFIG_MFC3_LABEL_KEY);
            item->setText(MFC_label);
            break;
        case 3:
            MFC_label = m_config.getValueForKey(CONFIG_MFC4_LABEL_KEY);
            item->setText(MFC_label);
            break;
        case 4:
            MFC_label = m_config.getValueForKey(CONFIG_MFC5_LABEL_KEY);
            item->setText(MFC_label);
            break;
        case 5:
            MFC_label = m_config.getValueForKey(CONFIG_MFC6_LABEL_KEY);
            item->setText(MFC_label);
            break;
        }
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



void EngineerTab::MFCRecipeValueChange(int mfcNumber)
{
    QString strRange;
    MFC* mfc = nullptr;

    mfc = m_controller.findMFCByNumber(mfcNumber);
    if (mfc) {
        strRange = mfc->getRangeQString();

        // clear the previous value
        m_pRecipeInputDialog->setTextValue("");
        m_pRecipeInputDialog->setWindowTitle("TEST!!");
        m_pRecipeInputDialog->setInputMode(QInputDialog::TextInput);
        m_pRecipeInputDialog->setProperty("MFC_Num", mfcNumber);

        QString msg = "Format xx.yy (max value: " + strRange + ")   ";

        m_pRecipeInputDialog->setLabelText(msg);

        // disconnect any previous connections
        disconnect(m_pRecipeInputDialog, &QInputDialog::accepted, 0, 0);

        connect(m_pRecipeInputDialog, &QInputDialog::accepted, this, &EngineerTab::Recipe_MFCSetpointAccepted);

        m_pRecipeInputDialog->setFocus();
        m_pRecipeInputDialog->show();
        m_pRecipeInputDialog->adjustSize();
        QApplication::processEvents();
    }
}

void EngineerTab::Recipe_MFCSetpointAccepted()
{
    bool ok;
    QString recipeStr = m_pRecipeInputDialog->textValue();
    int mfcNumber = m_pRecipeInputDialog->property("MFC_Num").toInt(&ok);

    if (ok && !recipeStr.isEmpty()) { // User entered a string and clicked OK
        MFC *mfc = m_controller.findMFCByNumber(mfcNumber);
        bool validNum;
        double enteredValue = recipeStr.toDouble(&validNum);
        double range = mfc->getRange();
        if (validNum && enteredValue <= range && enteredValue >= 0 && mfc) {
            mfc->setRecipeFlow(enteredValue);
        }
    }
}

// update the recipe cells
void EngineerTab::updateRecipeFlow(const int mfcNumber, const double recipeFlow)
{
    int row = mfcNumber - 1;

    QTableWidgetItem *item = m_pUI->tablewidgetRecipeParams->item(row, 1); // row, column
    if (item == nullptr) {
        item = new QTableWidgetItem;
        m_pUI->tablewidgetRecipeParams->setItem(row, 1, item); // row, column, item
    }
    item->setText(QString::number(recipeFlow, 'f', 1));
}

// update the recipe watts
void EngineerTab::recipeWattsChanged(int watts)
{
    QTableWidgetItem *item = m_pUI->tablewidgetRecipeParams->item(6, 1); // row, column
    if (item == nullptr) {
        item = new QTableWidgetItem;
        m_pUI->tablewidgetRecipeParams->setItem(6, 1, item); // row, column, item
    }
    item->setText(QString::number(watts));
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


void EngineerTab::stageParams_cellClicked(int row, int column)
{
    MFCRecipeValueChange(row + 1);
    /*if (column == 1) {
        switch (row) {
        case 0:
            // MFC1
            MFCRecipeValueChange(row + 1);
            break;
        case 1:
            // MFC2
            break;
        case 2:
            // MFC3
            break;
        case 3:
            // MFC4
            break;
        case 4:
            // MFC5
            break;
        case 5:
            // MFC6
            break;
        }
    }*/
}

void EngineerTab::recipeParams_cellClicked(int row, int column)
{
    if (column == 1) {
        MFCRecipeValueChange(row + 1);
        switch (row) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            MFCRecipeValueChange(row + 1);
            break;
        case 6:
            break;
        }
    }
}

