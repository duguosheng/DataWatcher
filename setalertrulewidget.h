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

private slots:
    void on_pushButton_2_clicked();
    void on_cBox_var1_stateChanged(int arg1);
    void on_cBox_var2_stateChanged(int arg1);
    void on_cBox_var3_stateChanged(int arg1);
    void on_pushButton_clicked();

private:
    Ui::SetAlertRuleWidget *ui;
    QString ruleStr;
    QByteArray ruleBtArray;
    QJsonDocument ruleJsonDoc;
    QString name;
    QString sql;
    QString expr;
    QString period;
    QString forTime;
    QString fromTime;
    QString annotations;
    static const QString valueType[];
    static const QString craneParams[];
    static const QString compareType[];
    static const QString unitPeriod[];
    static const QString unitFor[];
    static const QString unitTimeFrame[];
    void InitDialog();
    void BuildJson();
    bool GenerateRule();
//    QString FindSubString(const QString &str, const QString &regexp);
    QNetworkAccessManager *manager;

private slots:
    void ReplyFinished(QNetworkReply *reply);
};

#endif // SETALERTRULEWIDGET_H
