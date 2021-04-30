#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "tdenginedb.h"
#include "common.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);
    InitDialog();
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

/**
 * @brief SettingsDialog::on_pBtn_LogDir_clicked 槽函数：更改日志存放目录
 */
void SettingsDialog::on_pBtn_LogDir_clicked() {
    QString dir = QFileDialog::getExistingDirectory();
    setting->setValue("System/logDir", dir);
    ui->lineE_logDir->setText(dir);
}

/**
 * @brief SettingsDialog::InitDialog 初始化偏好设置界面
 */
void SettingsDialog::InitDialog() {
    setting->beginGroup("System");
    if (setting->value("autostart", false).toBool())
        ui->cBox_autostart->setCheckState(Qt::Checked);
    else
        ui->cBox_autostart->setCheckState(Qt::Unchecked);
    ui->lineE_logDir->setText(setting->value("logDir", QDir::currentPath() + "/log").toString());
    setting->endGroup();

    ui->lineE_passwd->setEchoMode(QLineEdit::Password);
    setting->beginGroup("Login");
    if (setting->value("saveuserinfo", false).toBool())
        ui->cBox_saveUserInfo->setCheckState(Qt::Checked);
    else
        ui->cBox_saveUserInfo->setCheckState(Qt::Unchecked);

    if (ui->cBox_saveUserInfo->isChecked()) {
        ui->lineE_userName->setText(setting->value("username", QString()).toString());
        ui->lineE_passwd->setText(setting->value("passwd", QString()).toString());
    } else {
        ui->label_userName->setEnabled(false);
        ui->lineE_userName->setEnabled(false);
        ui->label_passwd->setEnabled(false);
        ui->lineE_passwd->setEnabled(false);
    }
    setting->endGroup();

    setting->beginGroup("TDengine");
    ui->lineE_ipaddr->setText(setting->value("ipaddr", "1.15.111.120").toString());
    ui->lineE_port->setText(setting->value("port", 6030).toString());
    ui->lineE_dbName->setText(setting->value("dbname", "health_status").toString());
    setting->endGroup();

    setting->beginGroup("Grafana");
    ui->lineE_gIpaddr->setText(setting->value("ipaddr", "1.15.111.120").toString());
    ui->lineE_gPort->setText(setting->value("port", 3000).toString());
    setting->endGroup();

    setting->beginGroup("AlertManager");
    ui->lineE_aIpaddr->setText(setting->value("ipaddr", "1.15.111.120").toString());
    ui->lineE_aRulePort->setText(setting->value("ruleport", 8100).toString());
    ui->lineE_aAlertPort->setText(setting->value("alertport", 9093).toString());
    setting->endGroup();

}

void SettingsDialog::on_pBtn_confirm_clicked() {
    setting->beginGroup("System");
    if (ui->cBox_autostart->isChecked() == false) {
        setting->setValue("autostart", false);
    } else {
        setting->setValue("autostart", true);
    }
    setting->setValue("logDir", ui->lineE_logDir->text());
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

    setting->beginGroup("TDengine");
    setting->setValue("ipaddr", ui->lineE_ipaddr->text());
    setting->setValue("port", ui->lineE_port->text());
    setting->setValue("dbname", ui->lineE_dbName->text());
    setting->endGroup();

    setting->beginGroup("Grafana");
    setting->setValue("ipaddr", ui->lineE_gIpaddr->text());
    setting->setValue("port", ui->lineE_gPort->text());
    setting->endGroup();

    setting->beginGroup("AlertManager");
    setting->setValue("ipaddr", ui->lineE_aIpaddr->text());
    setting->setValue("ruleport", ui->lineE_aRulePort->text());
    setting->setValue("alertport", ui->lineE_aAlertPort->text());
    setting->endGroup();

    QMessageBox::information(this, tr("提示"), tr("配置将在下次启动时生效"));
    close();
}

void SettingsDialog::on_cBox_saveUserInfo_stateChanged(int state) {
    if (state == Qt::Checked) {
        ui->label_userName->setEnabled(true);
        ui->lineE_userName->setEnabled(true);
        ui->label_passwd->setEnabled(true);
        ui->lineE_passwd->setEnabled(true);
    } else {
        ui->label_userName->setEnabled(false);
        ui->lineE_userName->setEnabled(false);
        ui->label_passwd->setEnabled(false);
        ui->lineE_passwd->setEnabled(false);
    }
}

void SettingsDialog::on_pBtn_cancel_clicked() {
    close();
}
