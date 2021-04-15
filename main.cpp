#include "mainwindow.h"
#include "logindialog.h"
#include "common.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    log = new LogFile;
    SaveLog(QObject::tr("程序启动"));

    //登陆界面
    LoginDialog loginDlg;

    //验证用户信息
    if (loginDlg.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}
