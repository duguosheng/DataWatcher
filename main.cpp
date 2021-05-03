#include "mainwindow.h"
#include "logindialog.h"
#include "common.h"
#include "settingsdialog.h"
#include "tdenginedb.h"

#include <QApplication>
#include <QDebug>
#include <QThread>

void InitSettings();

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    InitSettings();
    //登陆界面
    LoginDialog loginDlg;

    //验证用户信息
    if (loginDlg.exec() == QDialog::Accepted) {
        //登陆成功，关闭登陆界面，开启主界面
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}

/**
 * @brief appAutoRun 设置开机自启动
 * @param bAutoRun 是否开机自启动
 */
void AppAutoStart(bool isAutoRun) {
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString applicationName = QApplication::applicationName();
    if (isAutoRun) {
        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue(applicationName, strAppPath);
    } else {
        reg.remove(applicationName);
    }
}

/**
 * @brief InitSettings 初始化系统配置
 */
void InitSettings() {
    QCoreApplication::setOrganizationName("DuGuosheng");
    QCoreApplication::setOrganizationDomain("dgs.zone");
    QCoreApplication::setApplicationName("Crane");

    //所有系统都以ini文件方式创建设置对象
    setting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                            QApplication::organizationName(), QApplication::applicationName());

    //向ini文件中不存在的配置项写入初始值
    setting->beginGroup("System");
    if (!setting->contains("logDir"))
        setting->setValue("logDir", QDir::currentPath() + "/log");
    if (!setting->contains("autostart"))
        setting->setValue("autostart", false);
    setting->endGroup();

    setting->beginGroup("Login");
    if (!setting->contains("saveuserinfo"))
        setting->setValue("saveuserinfo", false);
    if (!setting->contains("username"))
        setting->setValue("username", QString());
    if (!setting->contains("passwd"))
        setting->setValue("passwd", QString());
    setting->endGroup();

    setting->beginGroup("TDengine");
    if (!setting->contains("ipaddr"))
        setting->setValue("ipaddr", "1.15.111.120");
    if (!setting->contains("port"))
        setting->setValue("port", 6030);
    if (!setting->contains("dbname"))
        setting->setValue("dbname", "health_status");
    if (!setting->contains("sTable"))
        setting->setValue("sTable", "crane");
    setting->endGroup();

    setting->beginGroup("Grafana");
    if (!setting->contains("ipaddr"))
        setting->setValue("ipaddr", "1.15.111.120");
    if (!setting->contains("port"))
        setting->setValue("port", 3000);
    setting->endGroup();

    setting->beginGroup("AlertManager");
    if (!setting->contains("ipaddr"))
        setting->setValue("ipaddr", "1.15.111.120");
    if (!setting->contains("ruleport"))
        setting->setValue("ruleport", 8100);
    if (!setting->contains("alertport"))
        setting->setValue("alertport", 9093);
    setting->endGroup();

    //读取配置
    setting->beginGroup("System");
    AppAutoStart(setting->value("autostart", false).toBool());
    //创建日志读写对象
    runningLog = new LogFile(setting->value("logDir", QDir::currentPath() + "/log").toString());
    setting->endGroup();

    SaveLog(QObject::tr("程序启动"));
    SaveLog(QObject::tr("初始化配置信息开始"));

    setting->beginGroup("Login");
    if (setting->value("saveuserinfo", false).toBool()) {
        dbinfo.userName = setting->value("username", QString()).toString();
        dbinfo.passwd = setting->value("passwd", QString()).toString();
    }
    setting->endGroup();

    setting->beginGroup("TDengine");
    dbinfo.ip = setting->value("ipaddr", "1.15.111.120").toString();
    dbinfo.port = setting->value("port", 6030).toUInt();
    dbinfo.dbName = setting->value("dbname", "health_status").toString();
    setting->endGroup();

    SaveLog(QObject::tr("初始化配置信息完成"));

    //Grafana和AlertManager将在启动时读取配置
}
