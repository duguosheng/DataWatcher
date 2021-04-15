#include "filesystem.h"
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

FileSystem::FileSystem() {}

LogFile::LogFile() : logFile(nullptr) {
    QDateTime dt;
    InitLogDir();
    logFileName = "run_" + dt.toString("yyyyMMddhhmmss") + ".log";
}

bool LogFile::OpenLogFile() {
    logFile = new QFile(logFileName);

    //追加写模式打开，且将换行符转换为本地编码
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return true;
    }

    delete logFile;
    logFile = nullptr;
    return false;
}

void LogFile::CloseLogFile() {
    if (logFile->isOpen()) {
        logFile->close();
        delete logFile;
        logFile = nullptr;
    }
}

bool LogFile::InitLogDir() {
    logDir = QDir::currentPath() + "/log";

    if (!QDir().exists(logDir)) {
        if (!QDir().mkpath(logDir)) {
            return false;
        }
    }

    return true;
}

void LogFile::SaveLog(const QString &log) {
    if (OpenLogFile()) {
        QDateTime dt;
        QTextStream out(logFile);
        out << "时间: " <<
            dt.currentDateTime().toString("yyyy年MM月dd日hh时mm分ss秒")
            << " " << log << endl;
        CloseLogFile();
    }
}
