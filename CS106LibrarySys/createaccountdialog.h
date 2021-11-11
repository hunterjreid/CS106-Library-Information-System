#ifndef CREATEACCOUNTDIALOG_H
#define CREATEACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class createAccountDialog;
}

class createAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit createAccountDialog(QWidget *parent = nullptr);
    ~createAccountDialog();

private slots:
    void on_createAccountBtn_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::createAccountDialog *ui;
};

#endif // CREATEACCOUNTDIALOG_H
