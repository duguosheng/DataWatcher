#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
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
