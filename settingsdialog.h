#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include "common.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_pBtn_LogDir_clicked();
    void on_pBtn_confirm_clicked();
    void on_cBox_saveUserInfo_stateChanged(int arg1);
    void on_pBtn_cancel_clicked();

private:
    Ui::SettingsDialog *ui;
    void InitDialog();
};

#endif // SETTINGSDIALOG_H
