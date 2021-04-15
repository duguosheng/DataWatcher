#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    qDebug() << "close main window";

    if (taos)
        taos_close(taos);

    delete ui;
}

