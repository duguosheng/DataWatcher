#include "common.h"
#include <QDebug>

//日志文件管理
LogFile *runningLog = nullptr;
QReadWriteLock logLock;
//软件偏好设置
QSettings *setting = nullptr;

/**
 * @brief SaveLog 保存日志
 * @param logString 要保存的内容
 */
void SaveLog(const QString &logString) {
    if (runningLog) {
        logLock.lockForWrite();
        runningLog->SaveLog(logString);
        logLock.unlock();
    }
}

/**
 * @brief GetLogFileName 获取日志文件名
 * @return 日志文件名
 */
QString GetLogFileName() {
    return runningLog->GetLogFileName();
}
/**
 * @brief ProgramExit 清理运行环境，程序退出应调用此API
 */
void ProgramExit() {
    SaveLog(QObject::tr("程序退出"));

    //关闭数据库连接
    if (taos) {
        taos_close(taos);
        taos_cleanup();
        taos = nullptr;
    }

    //关闭日志写入
    if (runningLog) {
        delete runningLog;
        runningLog = nullptr;
    }

    //关闭配置
    if (setting) {
        delete setting;
        setting = nullptr;
    }
}
