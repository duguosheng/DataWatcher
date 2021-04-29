#include "logindialog.h"
#include "ui_logindialog.h"
#include "common.h"
#include <QDebug>

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
    ProgramExit();
    delete ui;
}

/**
 * @brief LoginDialog::ConnectTDengingServer 连接到TDengine服务端
 * @return 连接成功返回true，连接失败返回false
 */
bool LoginDialog::ConnectTDengingServer() {
    taos  = taos_connect("123.56.99.48",                            //IP地址
                         QStringToChar(ui->lineE_username->text()), //用户名
                         QStringToChar(ui->lineE_passwd->text()),   //密码
                         "health",                                  //连接的数据库名
                         6030);                                     //端口号

    if (taos) {
        SaveLog(tr("成功连接到服务器"));
        return true;
    }

    SaveLog(tr("登录失败"));
    ui->label_errorMsg->setStyleSheet("color:rgb(150,20,20)");
    ui->label_errorMsg->setText(tr("登录失败，请重试！"));
    qDebug() << " ";
    return false;
}

void LoginDialog::on_pBtn_loginConfirm_clicked() {
    ui->label_errorMsg->setStyleSheet("color:rgb(65,105,225)");
    ui->label_errorMsg->setText(tr("正在登陆..."));
    ui->label_errorMsg->repaint();

    if (ConnectTDengingServer())
        accept();
}

void LoginDialog::on_pBtn_loginCancel_clicked() {
    close();
}

void LoginDialog::ClearErrorMsg() {
    ui->label_errorMsg->clear();
}
