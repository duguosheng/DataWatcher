#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "settingsdialog.h"
#include "realtimedatawidget.h"
#include "showalertruledialog.h"
#include "setalertrulewidget.h"
#include "showreadonlytextdialog.h"

#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QLabel *label_authorInfo = new QLabel(this);
    label_authorInfo->setFrameStyle(QFrame::NoFrame | QFrame::Sunken);
    label_authorInfo->setText(tr("软件开源地址：www.github.com/duguosheng/crane"));
    ui->statusBar->addPermanentWidget(label_authorInfo);
}

MainWindow::~MainWindow() {
    ProgramExit();
    delete ui;
}

void MainWindow::on_action_P_triggered() {
    SettingsDialog *settingDlg = new SettingsDialog(this);
    //设置窗口为模态
    settingDlg->setModal(true);
    settingDlg->show();
}

void MainWindow::on_action_V_triggered() {
    if (centralWidget()) {
        centralWidget()->close();
        centralWidget()->deleteLater();
    }
    RealTimeDataWidget *dataWidget = new RealTimeDataWidget(this);
    setCentralWidget(dataWidget);
}

void MainWindow::on_action_Grafana_G_triggered() {
    QDesktopServices::openUrl(QUrl("http://" +
                                   setting->value("Grafana/ipaddr", "1.15.111.120").toString() + ":" +
                                   setting->value("Grafana/port", "3000").toString()));
}

void MainWindow::on_action_AlertManager_M_triggered() {
    QDesktopServices::openUrl(QUrl("http://" +
                                   setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                   setting->value("AlertManager/alertport", "9093").toString()));
}

void MainWindow::on_action_V_2_triggered() {
    ShowAlertRuleDialog *showRuleDlg = new ShowAlertRuleDialog(this);
    showRuleDlg->show();
}

void MainWindow::on_action_E_triggered() {
    if (centralWidget()) {
        centralWidget()->close();
        centralWidget()->deleteLater();
    }
    SetAlertRuleWidget *setRuleWidget = new SetAlertRuleWidget(this);
    setCentralWidget(setRuleWidget);
}

void MainWindow::on_action_L_triggered() {
    QString logFile = setting->value("System/logDir", QDir::currentPath() + "/log").toString()
                      + "/" + GetLogFileName();
    ShowReadOnlyTextDialog *showTextDlg = new ShowReadOnlyTextDialog(logFile, "运行日志" + GetLogFileName(), this);
    showTextDlg->show();
}

void MainWindow::on_action_U_triggered() {
    QString helpDoc = QDir::currentPath() + "/README.txt";
    ShowReadOnlyTextDialog *showTextDlg = new ShowReadOnlyTextDialog(helpDoc, "帮助文档", this);
    showTextDlg->show();
}

void MainWindow::on_action_C_triggered() {
    QMessageBox::information(this, "关于软件", "武汉大学动力与机械学院2017级本科毕业设计\n汽车起重机健康状态实时监测系统\n作者：杜国胜");
}
