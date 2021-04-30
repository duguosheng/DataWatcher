#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "tdenginedb.h"
#include "common.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
/**
 * @brief InitSettings 初始化系统配置
 */
void InitSettings() {
    //所有系统都以ini文件方式创建设置对象
    setting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                            "DuGuosheng", "Crane");

    //写入初始值
    setting->beginGroup("FileManage");
    if (!setting->contains("logDir"))
        setting->setValue("logDir", QDir::currentPath() + "/log");
    setting->endGroup();

    setting->beginGroup("TDengine");
    if (!setting->contains("ipaddr"))
        setting->setValue("ipaddr", "1.15.111.120");
    if (!setting->contains("port"))
        setting->setValue("port", 6030);
    if (!setting->contains("dbname"))
        setting->setValue("dbname", "health_status");
    setting->endGroup();

    setting->beginGroup("Login");
    if (!setting->contains("saveuserinfo"))
        setting->setValue("saveuserinfo", false);
    if (!setting->contains("username"))
        setting->setValue("username", QString());
    if (!setting->contains("passwd"))
        setting->setValue("passwd", QString());
    setting->endGroup();


    //读取配置
    setting->beginGroup("FileManage");
    log = new LogFile(setting->value("logDir", QDir::currentPath() + "/log").toString());
    setting->endGroup();

    setting->beginGroup("TDengine");
    dbinfo.ip = setting->value("ipaddr", "1.15.111.120").toString();
    dbinfo.port = setting->value("port", 6030).toUInt();
    dbinfo.dbName = setting->value("dbname", "health_status").toString();
    setting->endGroup();

    setting->beginGroup("Login");
    if (setting->value("saveuserinfo", false).toBool()) {
        dbinfo.userName = setting->value("username", QString()).toString();
        dbinfo.passwd = setting->value("passwd", QString()).toString();
    }
    setting->endGroup();
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);
    InitDialog();
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::on_pBtn_LogDir_clicked() {
    QString dir = QFileDialog::getExistingDirectory();
    setting->setValue("FileManage/logDir", dir);
    ui->lineE_logDir->setText(dir);
}

void SettingsDialog::InitDialog() {
    setting->beginGroup("FileManage");
    ui->lineE_logDir->setText(setting->value("logDir", QDir::currentPath() + "/log").toString());
    setting->endGroup();

    setting->beginGroup("TDengine");
    ui->lineE_ipaddr->setText(setting->value("ipaddr", "1.15.111.120").toString());
    ui->lineE_port->setText(setting->value("port", 6030).toString());
    ui->lineE_dbName->setText(setting->value("dbname", "health_status").toString());
    setting->endGroup();

    setting->beginGroup("Login");
    if (setting->value("saveuserinfo", false).toBool())
        ui->cBox_saveUserInfo->setCheckState(Qt::Checked);
    else
        ui->cBox_saveUserInfo->setCheckState(Qt::Unchecked);

    if (ui->cBox_saveUserInfo->isChecked()) {
        ui->lineE_userName->setText(setting->value("username", QString()).toString());
        ui->lineE_passwd->setText(setting->value("passwd", QString()).toString());
    } else {
        ui->lineE_userName->setEnabled(false);
        ui->lineE_passwd->setEnabled(false);
    }
    setting->endGroup();
}

void SettingsDialog::on_pBtn_confirm_clicked() {
    setting->beginGroup("FileManage");
    setting->setValue("logDir", ui->lineE_logDir->text());
    setting->endGroup();

    setting->beginGroup("TDengine");
    setting->setValue("ipaddr", ui->lineE_ipaddr->text());
    setting->setValue("port", ui->lineE_port->text());
    setting->setValue("dbname", ui->lineE_dbName->text());
    setting->endGroup();

    setting->beginGroup("Login");
    if (ui->cBox_saveUserInfo->isChecked() == false) {
        setting->setValue("username", QString());
        setting->setValue("passwd", QString());
    } else {
        setting->setValue("username", ui->lineE_userName->text());
        setting->setValue("passwd", ui->lineE_passwd->text());
    }
    setting->endGroup();

    QMessageBox::information(this, tr("提示"), tr("配置将在下次启动时生效"));
    close();
}

void SettingsDialog::on_cBox_saveUserInfo_stateChanged(int state) {
    if (state == Qt::Checked) {
        ui->lineE_userName->setEnabled(true);
        ui->lineE_passwd->setEnabled(true);
    } else {
        ui->lineE_userName->setEnabled(false);
        ui->lineE_passwd->setEnabled(false);
    }
}
