#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "settingsdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    ProgramExit();
    delete ui;
}

void MainWindow::InitSystem() {
    //初始化设置信息
}


void MainWindow::on_action_P_triggered() {
    SettingsDialog *settingDlg = new SettingsDialog(this);
    //设置窗口为模态
    settingDlg->setModal(true);
    settingDlg->show();
}
