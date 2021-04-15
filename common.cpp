#include "common.h"

//数据库连接句柄
TAOS *taos;
//日志文件管理
LogFile *log;

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
    log->SaveLog(logString);
}
