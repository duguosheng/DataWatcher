#include "logfile.h"
#include "common.h"
#include <QDateTime>
#include <QTextStream>

LogFile::LogFile(QObject *parent) : QObject(parent) {
    SetLogDir(QDir::currentPath() + "/log");
}

LogFile::LogFile(const QString &dir, QObject *parent)
    :  QObject(parent) {
    SetLogDir(dir);
}

bool LogFile::SetLogDir(const QString &dir) {
    if (dir.isEmpty())
        logDir = QDir::currentPath() + "/log";
    else
        logDir = dir;

    if (!QDir().exists(logDir)) {
        if (!QDir().mkpath(logDir)) {
            emit logFailure(tr("创建日志存储目录失败!"));
            return false;
        }
    }

    QDateTime dt;
    logFileName = "dgs-" + dt.currentDateTime().toString("yyyy_MM_dd-hh_mm_ss") + ".log";
    logFile.setFileName(logDir + "/" + logFileName);

    return true;
}


bool LogFile::OpenLogFile() {
    //追加写模式打开，且将换行符转换为本地编码
    return logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void LogFile::CloseLogFile() {
    if (logFile.isOpen()) {
        logFile.close();
    }
}


void LogFile::SaveLog(const QString &log) {
    if (OpenLogFile()) {
        QDateTime dt;
        QTextStream out(&logFile);

        out << tr("时间: ") <<
            dt.currentDateTime().toString(tr("yyyy年MM月dd日hh时mm分ss秒"))
            << " " << log << endl;
        CloseLogFile();
    }
}
