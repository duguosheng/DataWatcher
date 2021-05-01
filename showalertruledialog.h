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
    QNetworkAccessManager *manager;
    QString rules;

private slots:
    void ReplyFinished(QNetworkReply *reply);
    void on_pBtn_close_clicked();
    void on_pBtn_refresh_clicked();
    void on_pBtn_format_clicked(bool checked);
};

#endif // SHOWALERTRULEDIALOG_H
