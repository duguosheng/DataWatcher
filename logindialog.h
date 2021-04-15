#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    //检查用户信息
    bool ConnectTDengingServer();

private slots:
    void on_pBtn_loginConfirm_clicked();

    void on_pBtn_loginCancel_clicked();
    void ClearErrorMsg();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
