#ifndef SHOWALERTRULEDIALOG_H
#define SHOWALERTRULEDIALOG_H

#include <QDialog>

namespace Ui {
class ShowAlertRuleDialog;
}

class QNetworkReply;
class QNetworkAccessManager;

class ShowAlertRuleDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowAlertRuleDialog(QWidget *parent = nullptr);
    ~ShowAlertRuleDialog();

private:
    Ui::ShowAlertRuleDialog *ui;
    QNetworkAccessManager *getRuleManager;
    QNetworkAccessManager *delRuleManager;
    QString rules;
    QString delRuleName;
    QStringList ruleNames;
    void ExtractRuleNames();

private slots:
    void GetReplyFinished(QNetworkReply *reply);
    void DelReplyFinished(QNetworkReply *reply);
    void on_pBtn_close_clicked();
    void on_pBtn_refresh_clicked();
    void on_pBtn_format_clicked(bool checked);
    void on_pBtn_deleteRule_clicked(bool checked);
    void on_pBtn_confirmDelete_clicked();
};

#endif // SHOWALERTRULEDIALOG_H
