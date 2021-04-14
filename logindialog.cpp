#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    //密码框输入不显示明文
    ui->lineE_passwd->setEchoMode(QLineEdit::Password);

    //用户重新输入时，清除错误提示信息
    connect(ui->lineE_username, &QLineEdit::textChanged, this, &LoginDialog::ClearErrorMsg);
    connect(ui->lineE_passwd, &QLineEdit::textChanged, this, &LoginDialog::ClearErrorMsg);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

/**
 * @brief LoginDialog::CheckUserInfo 检查用户信息
 * @return 正确返回true，错误返回false
 */
bool LoginDialog::CheckUserInfo() {
    if (ui->lineE_username->text() == "dgs" && ui->lineE_passwd->text() == "000")
        return true;

    ui->label_errorMsg->setStyleSheet("color:rgb(150,20,20)");
    ui->label_errorMsg->setText(tr("用户名或密码错误！"));

    return false;
}

void LoginDialog::on_pBtn_loginConfirm_clicked() {
    //验证用户信息
    if (CheckUserInfo())
        accept();
}

void LoginDialog::on_pBtn_loginCancel_clicked() {
    close();
}

void LoginDialog::ClearErrorMsg() {
    ui->label_errorMsg->clear();
}
