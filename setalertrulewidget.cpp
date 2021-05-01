#include "setalertrulewidget.h"
#include "ui_setalertrulewidget.h"
#include "common.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

const QString SetAlertRuleWidget::valueType[] = {"AVG", "MAX", "MIN"};
const QString SetAlertRuleWidget::craneParams[] = {"h_temperature", "h_oilvolume",
                                                   "h_pressure_1", "h_flow_1", "h_vibration_1",
                                                   "h_pressure_2", "h_flow_2", "h_vibration_2",
                                                   "d_pressure", "d_temperature", "d_vibration"
                                                  };
const QString SetAlertRuleWidget::compareType[] = {">", "<"};
const QString SetAlertRuleWidget::unitPeriod[] = {"s", "m", "h", "d"};
const QString SetAlertRuleWidget::unitFor[] = {"s", "m", "h", "d"};
const QString SetAlertRuleWidget::unitTimeFrame[] = {"u", "a", "s", "m", "h", "d", "w"};

SetAlertRuleWidget::SetAlertRuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetAlertRuleWidget),
    manager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);
    InitDialog();
    connect(manager, &QNetworkAccessManager::finished, this, &SetAlertRuleWidget::ReplyFinished);
}

SetAlertRuleWidget::~SetAlertRuleWidget() {
    delete ui;
}

void SetAlertRuleWidget::BuildJson() {
    //构建json
    QJsonObject ruleJson;
    ruleJson.insert("name", name);
    ruleJson.insert("sql", sql);
    ruleJson.insert("expr", expr);
    ruleJson.insert("period", period);
    ruleJson.insert("for", forTime);
    QJsonObject annotationJson;
    annotationJson.insert("summary", annotations);
    ruleJson.insert("annotations", QJsonValue(annotationJson));

    ruleJsonDoc.setObject(ruleJson);
    ruleBtArray = ruleJsonDoc.toJson(QJsonDocument::Indented);
    ruleStr = QString(ruleBtArray);
}

bool SetAlertRuleWidget::GenerateRule() {
    name = ui->lineE_ruleName->text();
    if (ui->lineE_period->text().isEmpty() == false)
        period = ui->lineE_period->text() + unitPeriod[ui->coBox_period->currentIndex()];
    if (ui->lineE_for->text().isEmpty() == false)
        forTime = ui->lineE_for->text() + unitFor[ui->coBox_for->currentIndex()];
    if (ui->lineE_timeFrom->text().isEmpty() == false)
        fromTime = ui->lineE_timeFrom->text() + unitTimeFrame[ui->coBox_timeFrame->currentIndex()];
    annotations = ui->lineE_annotations->text();

    //生成SQL语句
    sql = "SELECT ";
    //读取变量
    if (ui->cBox_var1->isChecked()) {
        sql += valueType[ui->coBox_valueType1->currentIndex()] + "("
               + craneParams[ui->coBox_param1->currentIndex()] + ")"
               + " AS " + ui->lineE_alias1->text();
        expr = ui->lineE_alias1->text() + " "
               + compareType[ui->coBox_compare1->currentIndex()] + " "
               + ui->lineE_threshold1->text();
        // 只有在变量一设置了之后才能选定变量二
        if (ui->cBox_var2->isChecked()) {
            sql += ", " + valueType[ui->coBox_valueType2->currentIndex()] + "("
                   + craneParams[ui->coBox_param2->currentIndex()] + ")"
                   + " AS " + ui->lineE_alias2->text();
            expr += " && " + ui->lineE_alias2->text() + " "
                    + compareType[ui->coBox_compare2->currentIndex()] + " "
                    + ui->lineE_threshold2->text();
            // 只有在变量二设置了之后才能选定变量三
            if (ui->cBox_var3->isChecked()) {
                sql += ", " + valueType[ui->coBox_valueType3->currentIndex()] + "("
                       + craneParams[ui->coBox_param3->currentIndex()] + ")"
                       + " AS " + ui->lineE_alias3->text();
                expr += " && " + ui->lineE_alias3->text() + " "
                        + compareType[ui->coBox_compare3->currentIndex()] + " "
                        + ui->lineE_threshold3->text();
            }
        }
    }
    sql += " FROM " + setting->value("TDengine/dbname", "health_status").toString()
           + "." + setting->value("TDengine/sTable", "crane").toString()
           + " WHERE ts > now-" + fromTime + " GROUP BY TBNAME";

    //构建JSON
    BuildJson();

    //存在未填写的元素
    if (name.isEmpty() || period.isEmpty() || forTime.isEmpty() || fromTime.isEmpty()
            || annotations.isEmpty() || !ui->cBox_var1->isChecked())
        return false;

    return true;
}

void SetAlertRuleWidget::ReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "通知", "添加成功");
        SaveLog("向服务器配置告警规则成功");
    } else {
        QMessageBox::warning(this, "通知", "添加失败");
        SaveLog("向服务器配置告警规则失败");
    }
    reply->deleteLater();
}


void SetAlertRuleWidget::on_pushButton_2_clicked() {
    GenerateRule();
    ui->tBrow_showRule->setText(ruleStr);
}

void SetAlertRuleWidget::InitDialog() {
    ui->cBox_var1->setChecked(true);
    ui->cBox_var2->setChecked(false);
    ui->cBox_var3->setChecked(false);
    on_cBox_var1_stateChanged(Qt::Checked);
    on_cBox_var2_stateChanged(Qt::Unchecked);
    on_cBox_var3_stateChanged(Qt::Unchecked);

    ui->lineE_period->setValidator(new QIntValidator(0, 99999, this));
    ui->lineE_for->setValidator(new QIntValidator(0, 99999, this));
    ui->lineE_timeFrom->setValidator(new QIntValidator(0, 99999, this));
    ui->lineE_threshold1->setValidator(new QDoubleValidator(0, 9999999, 5, this));
    ui->lineE_threshold2->setValidator(new QDoubleValidator(0, 9999999, 5, this));
    ui->lineE_threshold3->setValidator(new QDoubleValidator(0, 9999999, 5, this));
}

void SetAlertRuleWidget::on_cBox_var1_stateChanged(int state) {
    if (state == Qt::Checked) {
        ui->coBox_valueType1->setEnabled(true);
        ui->coBox_param1->setEnabled(true);
        ui->label_alias1->setEnabled(true);
        ui->lineE_alias1->setEnabled(true);
        ui->coBox_compare1->setEnabled(true);
        ui->lineE_threshold1->setEnabled(true);
        ui->cBox_var2->setEnabled(true);
    } else {
        ui->coBox_valueType1->setEnabled(false);
        ui->coBox_param1->setEnabled(false);
        ui->label_alias1->setEnabled(false);
        ui->lineE_alias1->setEnabled(false);
        ui->coBox_compare1->setEnabled(false);
        ui->lineE_threshold1->setEnabled(false);

        ui->cBox_var2->setChecked(false);
        ui->cBox_var2->setEnabled(false);
        ui->cBox_var3->setChecked(false);
        ui->cBox_var3->setEnabled(false);
    }
}

void SetAlertRuleWidget::on_cBox_var2_stateChanged(int state) {
    //只有变量一可用才使能变量二
    if (state == Qt::Checked) {
        ui->coBox_valueType2->setEnabled(true);
        ui->coBox_param2->setEnabled(true);
        ui->label_alias2->setEnabled(true);
        ui->lineE_alias2->setEnabled(true);
        ui->coBox_compare2->setEnabled(true);
        ui->lineE_threshold2->setEnabled(true);
        ui->cBox_var3->setEnabled(true);
    } else {
        ui->coBox_valueType2->setEnabled(false);
        ui->coBox_param2->setEnabled(false);
        ui->label_alias2->setEnabled(false);
        ui->lineE_alias2->setEnabled(false);
        ui->coBox_compare2->setEnabled(false);
        ui->lineE_threshold2->setEnabled(false);

        ui->cBox_var3->setChecked(false);
        ui->cBox_var3->setEnabled(false);
    }
}

void SetAlertRuleWidget::on_cBox_var3_stateChanged(int state) {
    //只有变量二可用才使能变量三
    if (state == Qt::Checked) {
        ui->coBox_valueType3->setEnabled(true);
        ui->coBox_param3->setEnabled(true);
        ui->label_alias3->setEnabled(true);
        ui->lineE_alias3->setEnabled(true);
        ui->coBox_compare3->setEnabled(true);
        ui->lineE_threshold3->setEnabled(true);
    } else {
        ui->coBox_valueType3->setEnabled(false);
        ui->coBox_param3->setEnabled(false);
        ui->label_alias3->setEnabled(false);
        ui->lineE_alias3->setEnabled(false);
        ui->coBox_compare3->setEnabled(false);
        ui->lineE_threshold3->setEnabled(false);
    }
}

void SetAlertRuleWidget::on_pushButton_clicked() {
    if (GenerateRule()) {
        manager->post(QNetworkRequest(QUrl("http://" +
                                           setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                           setting->value("AlertManager/ruleport", "8100").toString()
                                           + "/api/update-rule")), ruleBtArray);
    } else {
        QMessageBox::warning(this, "格式错误", "配置生成错误\n请仔细检查是否有项目为空或填错");
    }
}