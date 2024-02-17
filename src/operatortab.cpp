#include "operatortab.h"
#include "ui_mainwindow.h"

OperatorTab::OperatorTab(Ui::MainWindow* ui)
    :
    m_pui(ui)
{
}

void OperatorTab::init()
{

}

void OperatorTab::hideTest()
{
    m_pui->pushButton_2->hide();
}
