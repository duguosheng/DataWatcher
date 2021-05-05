#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "settingsdialog.h"
#include "showrealtimedatawidget.h"
#include "showalertruledialog.h"
#include "setalertrulewidget.h"
#include "showreadonlytextdialog.h"
#include "execsqlwidget.h"

#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFileDialog>

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
    ClearCentral();
    delete ui;
    ProgramExit();
}

/**
 * @brief MainWindow::on_action_P_triggered 新建窗口打开设置面板
 */
void MainWindow::on_action_P_triggered() {
    SettingsDialog *settingDlg = new SettingsDialog(this);
    //设置窗口为模态
    settingDlg->setModal(true);
    settingDlg->show();
}

/**
 * @brief MainWindow::on_action_V_triggered 主窗口打开实时监控面板
 */
void MainWindow::on_action_V_triggered() {
    ClearCentral();
    ShowRealTimeDataWidget *dataWidget = new ShowRealTimeDataWidget(this);
    setCentralWidget(dataWidget);
}

/**
 * @brief MainWindow::on_action_Grafana_G_triggered 默认浏览器打开Grafana
 */
void MainWindow::on_action_Grafana_G_triggered() {
    SaveLog("网页打开Grafana");
    QDesktopServices::openUrl(QUrl("http://" +
                                   setting->value("Grafana/ipaddr", "1.15.111.120").toString() + ":" +
                                   setting->value("Grafana/port", "3000").toString()));
}

/**
 * @brief MainWindow::on_action_AlertManager_M_triggered 默认浏览器打开AlertManager
 */
void MainWindow::on_action_AlertManager_M_triggered() {
    SaveLog("网页打开AlertManager");
    QDesktopServices::openUrl(QUrl("http://" +
                                   setting->value("AlertManager/ipaddr", "1.15.111.120").toString() + ":" +
                                   setting->value("AlertManager/alertport", "9093").toString()));
}

/**
 * @brief MainWindow::on_action_V_2_triggered 新建窗口打开报警规则查看器
 */
void MainWindow::on_action_V_2_triggered() {
    ShowAlertRuleDialog *showRuleDlg = new ShowAlertRuleDialog(this);
    showRuleDlg->show();
}

/**
 * @brief MainWindow::on_action_E_triggered 主窗口打开报警规则设置器
 */
void MainWindow::on_action_E_triggered() {
    ClearCentral();
    SetAlertRuleWidget *setRuleWidget = new SetAlertRuleWidget(this);
    setCentralWidget(setRuleWidget);
}

/**
 * @brief MainWindow::on_action_L_triggered 新建窗口查看运行日志
 */
void MainWindow::on_action_L_triggered() {
    QString logFile = setting->value("System/logDir", QDir::currentPath() + "/log").toString()
                      + "/" + GetLogFileName();
    ShowReadOnlyTextDialog *showTextDlg = new ShowReadOnlyTextDialog(logFile, "运行日志" + GetLogFileName(), this);
    showTextDlg->SetReadLock(&logLock);
    showTextDlg->show();
}

/**
 * @brief MainWindow::on_action_U_triggered 新建窗口查看使用文档
 */
void MainWindow::on_action_U_triggered() {
    QString helpDoc = QDir::currentPath() + "/README.md";
    ShowReadOnlyTextDialog *showTextDlg = new ShowReadOnlyTextDialog(helpDoc, "帮助文档", this, "UTF-8");
    showTextDlg->show();
}

/**
 * @brief MainWindow::on_action_C_triggered 关于
 */
void MainWindow::on_action_C_triggered() {
    QMessageBox::about(this, "关于软件", "武汉大学动力与机械学院2017级本科毕业设计\n汽车起重机健康状态实时监测系统\n作者：杜国胜");
}

/**
 * @brief MainWindow::on_action_SQL_E_triggered 主界面打开编辑SQL面板
 */
void MainWindow::on_action_SQL_E_triggered() {
    ClearCentral();
    ExecSqlWidget *sqlWidget = new ExecSqlWidget(this);
    setCentralWidget(sqlWidget);
}

/**
 * @brief MainWindow::on_action_SQL_O_triggered 主界面打开编辑SQL面板，数据从文件读入
 */
void MainWindow::on_action_SQL_O_triggered() {
    QString sqlFileName = QFileDialog::getOpenFileName();
    if (sqlFileName.isEmpty())
        return;

    ClearCentral();

    ExecSqlWidget *sqlWidget = new ExecSqlWidget(this,
            ExecSqlWidget::OpenSqlFileMode, sqlFileName);
    setCentralWidget(sqlWidget);
}

/**
 * @brief MainWindow::on_action_C_2_triggered 清空主界面
 */
void MainWindow::on_action_C_2_triggered() {
    ClearCentral();
    setStyleSheet("QMainWindow{background-image: url(:/image/images/mainBackground.png);"
                  "background-repeat: no-repeat;"
                  "background-position: center;}");
}

void MainWindow::ClearCentral() {
    if (centralWidget()) {
        centralWidget()->close();
        centralWidget()->deleteLater();
    }
    setStyleSheet("");
}
