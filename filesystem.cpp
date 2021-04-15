#include "filesystem.h"
#include <QDateTime>
#include <QTextStream>

FileSystem::FileSystem() {}

LogFile::LogFile() {
    QDateTime dt;
    InitLogDir();
    logFile.setFileName(QString(logDir + "/run_" + dt.toString("yyyyMMddhhmmss") + ".log"));
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

bool LogFile::InitLogDir() {
    logDir = QDir::currentPath() + "/log";

    if (!QDir().exists(logDir)) {
        if (!QDir().mkpath(logDir)) {
            SaveLog(tr("创建日志存储目录失败!"));
            return false;
        }
    }

    return true;
}

void LogFile::SaveLog(const QString &log) {
    if (OpenLogFile()) {
        QDateTime dt;
        QTextStream out(&logFile);
        out << "时间: " <<
            dt.currentDateTime().toString("yyyy年MM月dd日hh时mm分ss秒")
            << " " << log << endl;
        CloseLogFile();
    }
}
