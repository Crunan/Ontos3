#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog),
    m_userEnteredPassword("")
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_buttonBox_accepted()
{
    m_userEnteredPassword = ui->lineEdit->text();
    emit userEnteredPassword();
}


void PasswordDialog::on_buttonBox_rejected()
{

}

