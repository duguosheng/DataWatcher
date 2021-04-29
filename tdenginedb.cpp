#include "tdenginedb.h"

/**
 * @brief TDengineDB::QueryDB 执行一句sql,无需结果返回
 * @param cmd 执行的SQL指令
 */
bool QueryDB(TAOS *handle, const QString &cmd) {
    const char *command = QStringToChar(cmd);
    int i;
    TAOS_RES *pSql = NULL;
    int32_t code = -1;

    //执行3次失败后退出
    for (i = 0; i < 3; i++) {
        if (NULL != pSql) {
            taos_free_result(pSql);
            pSql = NULL;
        }

        pSql = taos_query(handle, command);
        code = taos_errno(pSql);

        if (0 == code) {
            break;
        }
    }

    if (code != 0) {
        SaveLog("指令执行失败: " + cmd + ", 原因是: " + taos_errstr(pSql));
        taos_free_result(pSql);
        taos_close(handle);
        return false;
    }

    taos_free_result(pSql);
    return true;
}


