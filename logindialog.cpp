#include "logindialog.h"
#include "ui_logindialog.h"
#include "common.h"
#include "tdenginedb.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    //密码框输入不显示明文
    ui->lineE_passwd->setEchoMode(QLineEdit::Password);

    //自动填写账号密码
    setting->beginGroup("Login");
    if (setting->value("saveuserinfo", false).toBool()) {
        ui->cBox_saveUserInfo->setCheckState(Qt::Checked);
        ui->lineE_username->setText(setting->value("username", QString()).toString());
        ui->lineE_passwd->setText(setting->value("passwd", QString()).toString());
    }
    setting->endGroup();

    //用户重新输入时，清除错误提示信息
    connect(ui->lineE_username, &QLineEdit::textChanged, this, &LoginDialog::ClearErrorMsg);
    connect(ui->lineE_passwd, &QLineEdit::textChanged, this, &LoginDialog::ClearErrorMsg);
}

LoginDialog::~LoginDialog() {
    delete ui;
    ProgramExit();
}

/**
 * @brief LoginDialog::ConnectTDengingServer 连接到TDengine服务端
 * @return 连接成功返回true，连接失败返回false
 */
bool LoginDialog::ConnectTDengingServer() {
    dbinfo.userName = ui->lineE_username->text();
    dbinfo.passwd = ui->lineE_passwd->text();
    taos = ConnectDB(dbinfo);

    if (taos) {
        SaveLog(tr("成功连接到服务器"));
        return true;
    }

    SaveLog(tr("登录失败"));
    ui->label_errorMsg->setStyleSheet("color:rgb(150,20,20)");
    ui->label_errorMsg->setText(tr("登录失败，请重试！"));
    return false;
}

/**
 * @brief LoginDialog::on_pBtn_loginConfirm_clicked 确认登陆
 * 如果用户勾选了记住密码，则登陆成功后保存用户信息
 * 如果没有勾选，则清除用户信息
 */
void LoginDialog::on_pBtn_loginConfirm_clicked() {
    ui->label_errorMsg->setStyleSheet("color:rgb(65,105,225)");
    ui->label_errorMsg->setText(tr("正在登陆..."));
    ui->label_errorMsg->repaint();

    if (ConnectTDengingServer()) {
        //更新设置
        setting->beginGroup("Login");
        if (ui->cBox_saveUserInfo->isChecked()) {
            setting->setValue("saveuserinfo", true);
            setting->setValue("username", ui->lineE_username->text());
            setting->setValue("passwd", ui->lineE_passwd->text());
        } else {
            setting->setValue("saveuserinfo", false);
            setting->setValue("username", QString());
            setting->setValue("passwd", QString());
        }
        setting->endGroup();

        accept();
    }

}

void LoginDialog::on_pBtn_loginCancel_clicked() {
    close();
}

/**
 * @brief LoginDialog::ClearErrorMsg 用户重新输入时清除错误提示
 */
void LoginDialog::ClearErrorMsg() {
    ui->label_errorMsg->clear();
}

