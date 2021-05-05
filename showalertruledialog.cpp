#include "showalertruledialog.h"
#include "ui_showalertruledialog.h"
#include "common.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

ShowAlertRuleDialog::ShowAlertRuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowAlertRuleDialog),
    getRuleManager(new QNetworkAccessManager(this)),
    delRuleManager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);
    connect(getRuleManager, &QNetworkAccessManager::finished,
            this, &ShowAlertRuleDialog::GetReplyFinished);
    connect(delRuleManager, &QNetworkAccessManager::finished,
            this, &ShowAlertRuleDialog::DelReplyFinished);
    //立即刷新一次
    on_pBtn_refresh_clicked();
    //隐藏删除栏
    on_pBtn_deleteRule_clicked(false);
}

ShowAlertRuleDialog::~ShowAlertRuleDialog() {
    delete ui;
}

/**
 * @brief ShowAlertRuleDialog::ExtractRuleNames 抽取所有的规则名
 */
void ShowAlertRuleDialog::ExtractRuleNames() {
    ruleNames.clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rules.toUtf8());
    QVariantList list = jsonDoc.toVariant().toList();

    //http请求返回的是一个json数组，对数组中的所有元素逐个解析
    for (int i = 0; i < list.count(); ++i) {
        QVariantMap map = list[i].toMap();
        ruleNames.append(map["name"].toString());
    }
}

void ShowAlertRuleDialog::GetReplyFinished(QNetworkReply *reply) {
    rules = reply->readAll();
    ui->tBrow_alertRule->setText(rules.toUtf8());
    reply->deleteLater();
    //刷新后保持显示格式不变
    on_pBtn_format_clicked(ui->pBtn_format->isChecked());
}

void ShowAlertRuleDialog::DelReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "通知", "删除成功");
        SaveLog("删除告警规则：" + delRuleName + " 成功");

        on_pBtn_refresh_clicked();//删除后重新刷新
    } else {
        QMessageBox::warning(this, "通知", "删除失败");
        SaveLog("删除告警规则：" + delRuleName + " 失败");
    }
    delRuleName.clear();
    delRuleManager->clearAccessCache();
    reply->deleteLater();
}

void ShowAlertRuleDialog::on_pBtn_close_clicked() {
    deleteLater();
}

/**
 * @brief ShowAlertRuleDialog::on_pBtn_refresh_clicked 重新加载
 * 加载完成后将触发&QNetworkAccessManager::finished信号
 */
void ShowAlertRuleDialog::on_pBtn_refresh_clicked() {
    //重新加载
    getRuleManager->get(QNetworkRequest(QUrl("http://" +
                                        setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                        setting->value("AlertManager/ruleport", "8100").toString()
                                        + "/api/list-rule")));
}

/**
 * @brief ShowAlertRuleDialog::on_pBtn_format_clicked 格式化显示JSON
 * @param checked 是否启用格式化
 */
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

void ShowAlertRuleDialog::on_pBtn_deleteRule_clicked(bool checked) {
    ui->label_ruleName->setVisible(checked);
    ui->lineE_ruleName->setVisible(checked);
    ui->pBtn_confirmDelete->setVisible(checked);
    if (checked) {
        ui->pBtn_deleteRule->setText("取消删除");
    } else {
        ui->pBtn_deleteRule->setText("删除规则");
    }
}

void ShowAlertRuleDialog::on_pBtn_confirmDelete_clicked() {
    delRuleName = ui->lineE_ruleName->text();
    if (delRuleName.isEmpty())
        return;
    ExtractRuleNames();
    if (ruleNames.toSet().contains(delRuleName) == false) {
        QMessageBox::warning(this, "通知", "没有这个规则\n请检查您的输入");
        return;
    }
    delRuleManager->deleteResource(QNetworkRequest(QUrl("http://" +
                                   setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                   setting->value("AlertManager/ruleport", "8100").toString()
                                   + "/api/delete-rule?name=" + delRuleName)));
}
