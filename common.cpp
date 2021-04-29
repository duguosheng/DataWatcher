#include "common.h"

//数据库连接句柄
TAOS *taos = nullptr;
//日志文件管理
LogFile *log = nullptr;

/**
 * @brief QStringToChar 将QString转化为char*
 * @param qstr 待转化的字符串
 * @return 转换后的char*指针
 */
const char *QStringToChar(const QString &qstr) {
    return qstr.toLatin1().data();
}

/**
 * @brief SaveLog 保存日志
 * @param logString 要保存的内容
 */
void SaveLog(const QString &logString) {
    if (log == nullptr)
        log = new LogFile();

    log->SaveLog(logString);
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

}
