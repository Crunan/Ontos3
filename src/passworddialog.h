#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();

    QString getUserEnteredPassword() { return m_userEnteredPassword; }

signals:
    void userEnteredPassword();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PasswordDialog *ui;

    QString m_userEnteredPassword;
};

#endif // PASSWORDDIALOG_H
