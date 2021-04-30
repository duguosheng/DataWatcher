#include "tdenginedb.h"
#include <QDebug>


//数据库连接句柄
TAOS *taos = nullptr;
//数据库连接信息
struct TDengineConnectInfo dbinfo;
//参数绑定
TAOS_BIND params[16];
//存储查询数据
struct truck_crane_data craneData;

void InitTaosBindArray();
/**
 * @brief ConnectDB 连接到TDengine数据库服务端
 * @param dbconf 数据库连接配置信息
 * @return 数据库连接句柄，失败返回nullptr
 */
TAOS *ConnectDB(const TDengineConnectInfo &dbconf) {
//    InitTaosBindArray();

    return taos_connect(dbconf.ip.toStdString().c_str(),
                        dbconf.userName.toStdString().c_str(),
                        dbconf.passwd.toStdString().c_str(),
                        dbconf.dbName.toStdString().c_str(),
                        dbconf.port);
}

/**
 * @brief TDengineDB::QueryDB 执行一句sql,无需结果返回
 * @param cmd 执行的SQL指令
 */
bool QueryDB(const QString &cmd) {
    if (!taos)
        return false;
    const char *command = cmd.toStdString().c_str();
    int i;
    TAOS_RES *pSql = NULL;
    int32_t code = -1;

    //执行3次失败后退出
    for (i = 0; i < 3; i++) {
        if (NULL != pSql) {
            taos_free_result(pSql);
            pSql = NULL;
        }

        pSql = taos_query(taos, command);
        code = taos_errno(pSql);

        if (0 == code) {
            break;
        }
    }

    if (code != 0) {
        SaveLog("指令执行失败: " + cmd + ", 原因是: " + taos_errstr(pSql));
        taos_free_result(pSql);
        taos_close(taos);
        return false;
    }

    taos_free_result(pSql);
    return true;
}

void InitTaosBindArray() {
    params[0].buffer_type = TSDB_DATA_TYPE_TIMESTAMP;
    params[0].buffer_length = sizeof(craneData.time);
    params[0].buffer = &craneData.time;
    params[0].length = &params[0].buffer_length;
    params[0].is_null = NULL;

    params[1].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[1].buffer_length = sizeof(craneData.posi.longitude);
    params[1].buffer = &craneData.posi.longitude;
    params[1].length = &params[1].buffer_length;
    params[1].is_null = NULL;

    params[2].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[2].buffer_length = sizeof(craneData.posi.latitude);
    params[2].buffer = &craneData.posi.latitude;
    params[2].length = &params[2].buffer_length;
    params[2].is_null = NULL;

    params[3].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[3].buffer_length = sizeof(craneData.hydr.temperature);
    params[3].buffer = &craneData.hydr.temperature;
    params[3].length = &params[3].buffer_length;
    params[3].is_null = NULL;

    params[4].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[4].buffer_length = sizeof(craneData.hydr.oil_volume);
    params[4].buffer = &craneData.hydr.oil_volume;
    params[4].length = &params[4].buffer_length;
    params[4].is_null = NULL;

    params[5].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[5].buffer_length = sizeof(craneData.hydr.pressure[0]);
    params[5].buffer = &craneData.hydr.pressure[0];
    params[5].length = &params[5].buffer_length;
    params[5].is_null = NULL;

    params[6].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[6].buffer_length = sizeof(craneData.hydr.flow[0]);
    params[6].buffer = &craneData.hydr.flow[0];
    params[6].length = &params[6].buffer_length;
    params[6].is_null = NULL;

    params[7].buffer_type = TSDB_DATA_TYPE_INT;
    params[7].buffer_length = sizeof(craneData.hydr.vibration[0]);
    params[7].buffer = &craneData.hydr.vibration[0];
    params[7].length = &params[7].buffer_length;
    params[7].is_null = NULL;

    params[8].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[8].buffer_length = sizeof(craneData.hydr.pressure[1]);
    params[8].buffer = &craneData.hydr.pressure[1];
    params[8].length = &params[8].buffer_length;
    params[8].is_null = NULL;

    params[9].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[9].buffer_length = sizeof(craneData.hydr.flow[1]);
    params[9].buffer = &craneData.hydr.flow[1];
    params[9].length = &params[9].buffer_length;
    params[9].is_null = NULL;

    params[10].buffer_type = TSDB_DATA_TYPE_INT;
    params[10].buffer_length = sizeof(craneData.hydr.vibration[1]);
    params[10].buffer = &craneData.hydr.vibration[1];
    params[10].length = &params[10].buffer_length;
    params[10].is_null = NULL;

    params[11].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[11].buffer_length = sizeof(craneData.dyna.pressure);
    params[11].buffer = &craneData.dyna.pressure;
    params[11].length = &params[11].buffer_length;
    params[11].is_null = NULL;

    params[12].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[12].buffer_length = sizeof(craneData.dyna.temperature);
    params[12].buffer = &craneData.dyna.temperature;
    params[12].length = &params[12].buffer_length;
    params[12].is_null = NULL;

    params[13].buffer_type = TSDB_DATA_TYPE_INT;
    params[13].buffer_length = sizeof(craneData.dyna.vibration);
    params[13].buffer = &craneData.dyna.vibration;
    params[13].length = &params[13].buffer_length;
    params[13].is_null = NULL;

    params[14].buffer_type = TSDB_DATA_TYPE_INT;
    params[14].buffer_length = sizeof(craneData.groupid);
    params[14].buffer = &craneData.groupid;
    params[14].length = &params[14].buffer_length;
    params[14].is_null = NULL;

    params[15].buffer_type = TSDB_DATA_TYPE_BINARY;
    params[15].buffer_length = sizeof(craneData.type);
    params[15].buffer = &craneData.type;
    params[15].length = &params[15].buffer_length;
    params[15].is_null = NULL;
}

bool QueryDataFromCrane(const QString &tablename) {
    if (!taos)
        return false;
    QString sql = "SELECT LAST_ROW(*) FROM " + tablename;
    TAOS_STMT *stmt = taos_stmt_init(taos);
    taos_stmt_prepare(stmt, sql.toStdString().c_str(), 0);
    taos_stmt_bind_param(stmt, params);

    if (taos_stmt_execute(stmt) != 0) {
        SaveLog("执行SQL：" + sql + "失败！原因是" + taos_errstr(taos));
        return false;
    }
    taos_stmt_close(stmt);
    return true;
}

