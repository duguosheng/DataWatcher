#ifndef SETALERTRULEWIDGET_H
#define SETALERTRULEWIDGET_H

#include <QWidget>
#include <QJsonDocument>

namespace Ui {
class SetAlertRuleWidget;
}

class QNetworkReply;
class QNetworkAccessManager;

class SetAlertRuleWidget : public QWidget {
    Q_OBJECT

public:
    explicit SetAlertRuleWidget(QWidget *parent = nullptr);
    ~SetAlertRuleWidget();

private:
    Ui::SetAlertRuleWidget *ui;
    //以不同方式存储最终的规则信息
    QString ruleStr;
    QByteArray ruleBtArray;
    QJsonDocument ruleJsonDoc;

    //JSON中需要填写的各个字段
    QString name;
    QString sql;
    QString expr;
    QString period;
    QString forTime;
    QString fromTime;
    QString annotations;

    //将界面中的CoBox的值映射到字符串常量
    static const QString valueType[];
    static const QString craneParams[];
    static const QString compareType[];
    static const QString unitPeriod[];
    static const QString unitFor[];
    static const QString unitTimeFrame[];

    void InitDialog();
    void BuildJson();
    bool GenerateRule();
    QNetworkAccessManager *manager;

private slots:
    void ReplyFinished(QNetworkReply *reply);
    void on_pushButton_2_clicked();
    void on_cBox_var1_stateChanged(int state);
    void on_cBox_var2_stateChanged(int state);
    void on_cBox_var3_stateChanged(int state);
    void on_pushButton_clicked();
};

#endif // SETALERTRULEWIDGET_H
