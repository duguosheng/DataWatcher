#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "settingsdialog.h"
#include "realtimedatawidget.h"
#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>


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
    if (centralWidget())
        centralWidget()->deleteLater();
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
    if (centralWidget())
        centralWidget()->deleteLater();
}
