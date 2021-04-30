#include "common.h"

//日志文件管理
LogFile *log = nullptr;
//软件偏好设置
QSettings *setting = nullptr;

/**
 * @brief SaveLog 保存日志
 * @param logString 要保存的内容
 */
void SaveLog(const QString &logString) {
    if (log) {
        log->SaveLog(logString);
    }
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
    if (log) {
        delete log;
        log = nullptr;
    }

    //关闭配置
    if (setting) {
        delete setting;
        setting = nullptr;
    }
}
