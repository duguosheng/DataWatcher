#include "showalertruledialog.h"
#include "ui_showalertruledialog.h"
#include "common.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

ShowAlertRuleDialog::ShowAlertRuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowAlertRuleDialog),
    manager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &ShowAlertRuleDialog::ReplyFinished);
    manager->get(QNetworkRequest(QUrl("http://" +
                                      setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                      setting->value("AlertManager/ruleport", "8100").toString()
                                      + "/api/list-rule")));
}

ShowAlertRuleDialog::~ShowAlertRuleDialog() {
    delete ui;
}

void ShowAlertRuleDialog::ReplyFinished(QNetworkReply *reply) {
    rules = reply->readAll();
    //将接收到的JSON数据格式化后显示在textBrower
    //QJsonDocument jsonDoc = QJsonDocument::fromJson(rules.toUtf8());
    //QString fmtJsonStr = jsonDoc.toJson(QJsonDocument::Indented);
    ui->tBrow_alertRule->setText(rules.toUtf8());
    reply->deleteLater();
    //刷新后保持显示格式不变
    on_pBtn_format_clicked(ui->pBtn_format->isChecked());
}

void ShowAlertRuleDialog::on_pBtn_close_clicked() {
    deleteLater();
}

void ShowAlertRuleDialog::on_pBtn_refresh_clicked() {
    //重新加载
    manager->get(QNetworkRequest(QUrl("http://" +
                                      setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                      setting->value("AlertManager/ruleport", "8100").toString()
                                      + "/api/list-rule")));
}

void ShowAlertRuleDialog::on_pBtn_format_clicked(bool checked) {
    if (checked) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(rules.toUtf8());
        QString fmtJsonStr = jsonDoc.toJson(QJsonDocument::Indented);
        ui->tBrow_alertRule->setText(fmtJsonStr);
        ui->pBtn_format->setText(tr("原格式显示JSON"));
    } else {
        ui->tBrow_alertRule->setText(rules.toUtf8());
        ui->pBtn_format->setText(tr("缩进显示JSON"));
    }
}
